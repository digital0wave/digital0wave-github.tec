//ELEX 8030 Lab 11
//Jin Won Seo
#include "pwm.h"
#include "main.h"
#include "Servo.h"
#include "malloc.h"
#include "TestPin.h"
#include "HC12Rti.h"
#include "HC12Spi.h"
#include "TestTask.h"
#include "LTC1404.h"
#include "hc12ictlr.h"
#include "HC12SciBuf.h"
#include "AppCmdProc.h"

#define E_CLOCK 8000000//8 Mhz
#define PRESCALE_1 1//prescale set to 1 (default)
#define PRESCALE_2 2//prescale set to 2
#define PRESCALE_4 4//4

void memory_handler()
{
    //fail to allocate memory
	 //do something later
    
}

int main()
{
	//priority table
	enum priorities
	{
		priSchedLock,
		priTimerTask,		
		priLTC1404Task,
		priServoTask,
		priServoListTask,
		priTestTask,
		priCmdProc
		//priInitTask //~0
	};

	HC12SciBuf hc12buf(100,100, sci0);//rx,tx len, sci port 0
	
	HC12Ictlr ic;
	HC12Rti rti(ic, 1024, priTimerTask);
	
	//PWM Frequency : 2Khz
	PwmDriver pwmDrv(ic, E_CLOCK / PRESCALE_1 / 2000);// Desire PWM period(ticks) = E_CLOCK/PRESCALE/PWM Frequency
	//two channels : TEC, Resistor
	PwmChannel pwmTec(pwmDrv, 0); //for TEC
	PwmChannel pwmRes(pwmDrv, 1);//for resistor

	HC12Spi spi(spi0);
	LTC1404 ltc1404(1024, priLTC1404Task, spi);
	
	Servo servo(ltc1404, pwmTec, 1024, priServoTask, 21 , 1, 1, 0, 50); //pGain, iGain, dGain, iLimit

//	TestTask t1(256, priTestTask, 2, 5, 10);//stack size, port, times, interval
//	TestTask t2(256, priTestTask, 3, 7, 20);
	TestTask t3(256, priTestTask, 4, 3, 10);

/*
	TestTask t1(256, priTestTask, 2, 200, 10);//stack size, port, times, interval
	TestTask t2(256, priTestTask, 3, 100, 20);
	TestTask t3(256, priTestTask, 4, 300, 10);
*/
	AppCmdProc acp(&hc12buf, rti, ltc1404, pwmTec, pwmRes, servo, 1024, priCmdProc, priServoListTask);
//	AppCmdProc acp(&hc12buf, rti,  1024, priCmdProc);

	while(true)
	{
			
//		acp.main();
//		Task::yield();
		asm("wai"); //idle task
	}


	while(1);

}

