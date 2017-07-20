
#include "os_timer.h"



static T_OS_TIMER * head_handle = NULL;
static uint32_t timer_tick = 0;
	
void OS_TimerInit(T_OS_TIMER * timer, 
                                     timeout_callback timeout_func, 
                                     void * param,
                                     uint32_t time,
                                     uint8_t flag)
{
   if(!timer){ TIMER_DEBUG("null timer, %s, %d\n", __FILE__, __LINE__); return; }
   
   timer->timeout_func = timeout_func;
   timer->param = param;
   timer->init_tick = time;
   timer->timeout_tick = os_get_tick() + time;
   timer->flag = flag;
}



OS_RESULT OS_TimerStart(T_OS_TIMER * timer)
{
   T_OS_TIMER * target = head_handle;

   if(!timer){ TIMER_DEBUG("null timer, %s, %d\n", __FILE__, __LINE__); return OS_ERROR; }
   
   while(target)
   {
      if(target == timer)
	  {
	     target->flag |= OS_TIMER_FLAG_ACTIVATED;
		 TIMER_DEBUG("timer 0x%lx existed\r\n", (uint32_t)(timer));
		 return OS_OK;
	  }
	  target = target->next;
   }
   
   timer->next = head_handle;
   head_handle = timer;  // pointer to the last timer
   timer->flag |= OS_TIMER_FLAG_ACTIVATED;
   	
   TIMER_DEBUG("handle = 0x%lx, flag = 0x%lx\r\n", (uint32_t)head_handle, (uint32_t)timer->flag);
   
   return OS_OK;
}

void OS_TimerStop(T_OS_TIMER * timer)
{
   T_OS_TIMER * cur;

   if(!timer){ TIMER_DEBUG("null timer, %s, %d\n", __FILE__, __LINE__); return ; }
   
   for(cur = head_handle; cur; cur = cur->next)
   {
      
      if(cur == timer)
      {
         cur->flag &= ~ OS_TIMER_FLAG_ACTIVATED;
         TIMER_DEBUG("stop timer: 0x%lx\r\n", (uint32_t)(cur));
      }
   }
}

// return: 1: yes;  0: not stop
uint8_t OS_TimerIsStop(T_OS_TIMER * timer)
{
   if(!timer){ TIMER_DEBUG("null timer, %s, %d\n", __FILE__, __LINE__); return 0; }
   
   return ( !(timer->flag  & OS_TIMER_FLAG_ACTIVATED));
}

void OS_TimerCheck(void)
{
   T_OS_TIMER *cur = NULL;
   
   for(cur = head_handle; cur; cur = cur->next)
   {
      if(os_get_tick() >= cur->timeout_tick &&
	  	(cur->flag & OS_TIMER_FLAG_ACTIVATED))	
      {
		   TIMER_DEBUG("timer tick out: %ld ms\r\n", os_get_tick());
		   if(! (cur->flag & OS_TIMER_FLAG_PERIODIC))
		   {
		       cur->flag &= ~ OS_TIMER_FLAG_ACTIVATED;
		   }
		   else
		   {
		      cur->timeout_tick = os_get_tick() + cur->init_tick;
		   }
		   if(cur->timeout_func)cur->timeout_func(cur->param);
      }
   }
}

void OS_TimerTickIncrease(uint32_t tick)
{
   timer_tick++;
}

#if 0
void OS_TimerTask(void)
{
   if(os_get_tick() > (timer_tick + 10))  // 10 ms
   {
      timer_tick = os_get_tick();
      OS_TimerCheck();
   }
}
#endif

#if 0
void os_timer_setfn(os_timer_t *timer, os_timer_func_t * func, void *parg)
{
   OS_TimerInit(timer, func, parg, 0, 0);
}
#endif

// tick: how many ticks when time out, is_repeat: 1: repeat call back; 0: just one call back
void os_timer_arm(os_timer_t * timer, uint32_t tick, uint8_t is_repeat)
{
   if(!timer){ TIMER_DEBUG("null timer, %s, %d\n", __FILE__, __LINE__); return ; }
   
   timer->init_tick = tick;
   timer->timeout_tick = os_get_tick() + tick;
   if(is_repeat)
   {
      timer->flag |= OS_TIMER_FLAG_PERIODIC;  // timer repeat
   }
   OS_TimerStart(timer);
}



