//lab12
//Jin Won Seo
#ifndef 	SERVO_CONFIG_CMD_H
#define SERVO_CONFIG_CMD_H

#include "Cmd.h"
#include "Task.h"
#include "Servo.h"
#include "TimerTask.h"

class ServoConfigCmd : public Cmd
{
	Servo& servo;
	
	virtual const char* parse(istream& is, ostream& os)
	{
		char str[5];//string
		int  param =0; 
		float val = 0;
		//get a copy o f config obj
		Servo::Config c(servo);//call operator Config
		//get a copy o f status obj
//		Servo::Status s(servo);//call operator Config		

		if((is >> ws).eof())
		{
			os << c ;
			//os << s ;
			return 0;
		}
		//collect args 
		is >> str ;	
		is >> val;// value
		param = str[0];

		if(strcmp(str, "conf") == 0)
		{
			os <<  endl << "get config : " <<c;
		}else
		{
			
		//set usr input parameter values 
			switch (param)
			{
				case 's':
					c.setpoint = val;
					 os << endl << "set setpoint: " << val; 
					break;
				case 'p':
					c.pGain = val;
					 os << endl << "set proportional gain: " << val; 
					break;
				case 'i':
					c.iGain =val;
					 os << endl << "set integral gain: " << val; 
					break;
				case 'd':
					c.dGain = val;
					 os << endl << "set derivative gain: " << val; 
					break;
				case 'l'://iLimit
					c.iLimit = val;
					 os << endl << "set integral gain limit: " << val; 
					break;
				default:
					return "unknown paramters";			
			}		
			servo = c; //call operator =(Config& conf);
		}
		return 0; //no error			
	}//end parse 	
	
public:
	ServoConfigCmd(const char* name, Servo& sv) : Cmd(name), servo(sv) {}
	
};// end ServoConfigCmds

class ServoDisplayCmd :  public Cmd, public Task
{
//	streambuf* sbuf;//keep the order as these members are initialized
	//show servo params for each iteration 
	ostream os;
	istream is;

	Servo& servo;
	Semaphore running;//signal to display
	
	virtual void main();//for filitering
	//display one samle of servo status 
	virtual void display(ostream& os, Servo& servo) = 0;
	
	virtual const char* parse(istream& , ostream& os)
	{
		//char* c;
		os << endl;
		running.signal();
		//c = reinterpret_cast<char *>(is.get());//get one char from keyboard
		is.get();
		running.wait();
		//char *cp = reinterpret_cast<char *>(addr + i);
		//return c;// or just return 0
		return 0;
	}

public :
	//ctor Note: get serial port streambuf to initialize is, os 
	ServoDisplayCmd(const char* name, streambuf* sb, Servo& s, size_t stackSize, uint16 priority):  Cmd(name),  Task(stackSize, priority), os(sb), is(sb), servo(s) 
	{
		//os << "stackSize : " <<stackSize << endl;
		fork();
	}
	virtual ~ServoDisplayCmd(){}
	
};//end ServoDisplayCmd

//display periodically

void ServoDisplayCmd::main()
{
	while(true)
	{
		TimerTask::delay(1000);
		running.wait();
		//os << "ServoDisplayCmd::main()" << endl;
		display(os, servo);
		running.signal();
	}
}

class ServoListCmd: public ServoDisplayCmd
{

	virtual void display(ostream& os, Servo& servo)
	{
		Servo::Status s(servo);//call operator Status
//		Servo::Config c(servo);//call operator Status		
		os << " " << s << endl;
//ad		os << " " << c << endl;
	}
	
public:
	ServoListCmd(const char* name, streambuf* sb, Servo& s, size_t stackSize, uint16 priority) 
	:  ServoDisplayCmd(name, sb,  s, stackSize, priority){}//ctor
	virtual ~ServoListCmd(){}
};//end ServoListCmd

#endif
