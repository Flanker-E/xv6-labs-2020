// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;
struct {
  int inited;
  struct spinlock lock;
  uint16 count[(PHYSTOP+4096)/4096];
}refcount;

void
kinit()
{
  refcount.inited = 0;
  initlock(&kmem.lock, "kmem");
  initlock(&refcount.lock, "refcount");
  printf("kinit\n");
  freerange(end, (void*)PHYSTOP);
  printf("kinit\n");
  
  memset((char*)refcount.count, 0, sizeof(refcount.count));
  refcount.inited = 1;
}

void
krefinc(void *pa)
{
  uint64 ind=(uint64)pa/4096;
  
  acquire(&refcount.lock);
  // printf("refinc %d count %d\n",ind,refcount.count[ind]);
  refcount.count[ind]++;
  release(&refcount.lock);
}

void
krefdec(void *pa)
{
  uint64 ind=(uint64)pa/4096;
  acquire(&refcount.lock);
  // if(refcount.inited==1)
  //   printf("refdec %d count %d\n",ind,refcount.count[ind]);
  if(refcount.count[ind]==0 && refcount.inited==1)
    panic("krefdec:equals to zero");
  else
    refcount.count[ind]--;
  if(refcount.count[ind]==0 || refcount.inited==0){
    struct run *r;
    memset(pa, 1, PGSIZE);

    r = (struct run*)pa;

    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
  }
  release(&refcount.lock);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  krefdec(pa);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r){
    memset((char*)r, 5, PGSIZE); // fill with junk
    krefinc(r);
    }
  return (void*)r;
}
