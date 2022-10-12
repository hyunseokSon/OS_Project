#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

#define TIME_SLICE 10000000
#define NULL ((void *)0)


// proc 구조체는 ptable이라는 proc구조체의 배열로 관리됨.
// xv6에서 최대로 생성될 수 있는 프로세스는 64개이다.
struct {
  struct spinlock lock;
  struct proc proc[NPROC];
  
  // 1. need to add code...
  long long min_priority;
} ptable;

static struct proc *initproc;

//weight 값 추가.
int weight = 1;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

struct proc *ssu_schedule()
{
	struct proc *p;
	struct proc *ret = NULL;

	//2. you need to add code ...
	for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
		if (p->state == RUNNABLE) {
			if(ret == NULL || ( ret->priority > p->priority )) {
				ret = p;
			}
		}
	}

	// 과제3-3번  Code...
#ifdef DEBUG
	if(ret)
		cprintf("PID : %d, NAME : %s, WEIGHT : %d, PRIORITY : %d\n", ret->pid, ret->name, ret->weight, ret->priority);
#endif
	return ret;
}

void update_priority(struct proc *proc)
{
	//4. you need to add code... finish!!
	proc -> priority = proc->priority + (TIME_SLICE / proc->weight);
}

void update_min_priority()
{
	struct proc *min = NULL;
	struct proc *p;
	//5. you need to add code.. finish!!

	for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
		if (p->state == RUNNABLE) {
			if (min == NULL || (min->priority > p->priority))
				min = p;
		}
	}
	if (min != NULL)
		ptable.min_priority = min->priority;
}

void assign_min_priority(struct proc *proc)
{
	//6. you need to add code... finish!!
	proc->priority = ptable.min_priority;
}

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int
cpuid() {
  return mycpu()-cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
  int apicid, i;
  
  if(readeflags()&FL_IF)
    panic("mycpu called with interrupts enabled\n");
  
  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc*
myproc(void) {
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void) // 신규 생성되는 child process에 해당하는 함수.
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) // proc배열에 신규 프로세스를 할당할 공간이 있는지 조사한다.
    if(p->state == UNUSED) // 사용하지 않고 있는 것이 있다면 found로 이동한다.
      goto found; 

  release(&ptable.lock);
  return 0;

found:
  //7. you need to add code..
  p->weight = weight++; // 프로세스 생성 순서에 따라 1부터 차례대로 증가.

  p->state = EMBRYO; // 사용하고 있지 않은 배열공간이 있으면 우선 해당 공간을 EMBRYO 상태로 정의한다.
  p->pid = nextpid++; 

  assign_min_priority(p); // OSLAB 

  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){ // 신규 proc 구조체에 커널스택 메모리를 할당한다.
    p->state = UNUSED; // 실패하면 proc구조를 다시 원래 UNUSED 상태로 변경한다.
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  // 여기서부터는 return을 위한 stack을 수동으로 넣어주는 부분이다.
  sp -= sizeof *p->tf; // tf 사이즈만큼 stack pointer가 이동한다.
  p->tf = (struct trapframe*)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret; // traplet의 주소를 넣는다. 아래의 forkret이 수행된 이후 trapret으로 return된다.

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret; // 신규 proc context의 eip에 forkret 주소를 넣는다. 이러면 child는 fork함수 종료 후 forkret -> trapret으로 이동한다.

  return p;
}

//PAGEBREAK: 32
// Set up first user process.

//allocproc()함수를 호출하여 할당할 수 있는 proc구조체가 있는지 확인하고, 있으면 할당받는다.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  ptable.min_priority= 3; // 시작 시 priority = 3 으로 넣겠다.

  p = allocproc();
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;
  struct proc *curproc = myproc();

  sz = curproc->sz;
  if(n > 0){
    if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  } else if(n < 0){
    if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  curproc->sz = sz;
  switchuvm(curproc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np; // 새로운 proc 구조체를 선언한다.(==Linux의 task_struct와 동일)
  struct proc *curproc = myproc();

  // Allocate process.
  // 새로운 구조체가 할당할 수 있는 배열을 탐색하여 할당한다.
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy process state from proc.
  if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){ // 신규 자식의 proc 구조체에 현재 proc의 내용을 복사한다.
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc; // 신규 np의 부모를 proc으로 설정한다.
  *np->tf = *curproc->tf; // 신규 np의 trapframe(레지스터 등 정보 저장)에 부모의 것을 복사한다.

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0; // 자식 np의 trapframe의 %eax값을 0으로 지정한다.
  				   // 자식 프로세스는 forkret 후 trapret으로 돌아갈 때 리턴값으로 eax 값을 사용하게 된다.

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]); 
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid; // 자식의 pid를 부모에게 전달한다.

  acquire(&ptable.lock);

  np->state = RUNNABLE; // 자식의 프로세스 상태를 RUNNABLE로 지정한다.

  release(&ptable.lock);

  return pid; // 이때 부모는 자식 프로세스의 pid를 가지고 리턴한다. (fork가 trap에 걸려서 온 것이니 trapret 한다.)
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *curproc = myproc();
  struct proc *p;
  int fd;

  if(curproc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();
  
  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.

// ptable을 loop를 돌면서 running 가능한 프로세스를 찾아서 선택함. 
// 이때 ptable을 lock을 걸어야 하나의 process를 schedule()함수에서 리턴을 할 수 있음.
// lock을 걸지 않으면 table 탐색 중 다른 process가 I/O 등으로 schedule() 함수를 부르면 ptable을 또 탐색할 것임.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;
  
  for(;;){
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
	// ptable을 돌며 실행 가능한 process를 찾는다.
    acquire(&ptable.lock);

	p = ssu_schedule(); // OSLAB

	if (p == NULL) {
		release(&ptable.lock);
		continue;
	}

	c->proc = p;

	// OS는 실행을 위해 프로세스 정보를 load함.
	switchuvm(p); 	// uvm은 사용자
	
	// 프로세스를 로드하면 프로세스는 실행중으로 표시됨.
	p->state = RUNNING;

	//프로세서가 이를 실행하도록 전환.
	swtch(&(c->scheduler), p->context);

	// 프로세스가 스케줄러로 돌아올 때 (swtch가 일어난 이후) 커널은 그것의 메모리를 load함.
	switchkvm();	// kvm은 커널

	// 8. you need to add code,,,
	update_priority(p);
	update_min_priority();

	c->proc=0;
	release(&ptable.lock);

  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
  int intena;
  struct proc *p = myproc();

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(mycpu()->ncli != 1)
    panic("sched locks");
  if(p->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  myproc()->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();
  
  if(p == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

// 9. need to add code...
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan) {
      p->state = RUNNABLE;

  assign_min_priority(p); //RUNNABLE 된 process에게 우선순위를 할당한다.
	}
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

void do_weightset(int weight) // OSLAB
{
	acquire(&ptable.lock);
	myproc()->weight = weight;
	release(&ptable.lock);
}


