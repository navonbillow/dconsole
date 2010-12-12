#include "dconsole/common.h"
#include "dconsole/console.h"
#include "dconsole/command.h"

namespace dconsole {

    int testVar;

void setTestVar(int v)
{
    testVar = v;
}
int getTestVar(void)
{
    return testVar;
}

MyString f_hw()
{
    return "Hello, World!";
}

void f_voidretval()
{
	return ;
}

int f_intretval()
{
	return 42;
}

MyString f_string_p1Intp2Int(int , int )
{
	return "std::string: you've added two numbers :-)";
}

const char* f_constcharptr_p1Intp2Int(int , int )
{
	return "const char*: you've added two numbers :-)";
}

char* f_charptr_p1Intp2Int(int , int )
{
	return "char*: you've added two numbers :-)";
}

void f_void_intint(int, int)
{
	int a=10;
	(void)a;
	return;
}

MyString f_intfloatUD(int i1, float f2, int i3)
{
    MyString str1;
	MyString str2;
	MyString ud;

	std::stringstream inout;
	std::stringstream inout2;
	inout<<i1;
	inout>>str1;
	inout.clear();

	inout<<f2;
	inout>>str2;
	inout.clear();
	
	inout<<i3;
	inout>>ud;

	str1.append(" + ");
	str1.append(ud);
	str1.append("(user data)");

    if(i1 + i3 < f2)
        str1.append(" < ");
    else
        str1.append(" > ");
    str1.append(str2);

    return str1;
}

bool load_world(unsigned int key, bool async)
{
    return true;
}

int f_intintint(int i1, int i2, int i3)
{
    return (i1+i2)*i3;
}

float f_intfloat(int i, float f)
{
    return i+f;
}

int f_intptr(int* pi)
{
    return *pi;
}

int f(float f)
{
    return (int)(f+0.5f);
}

void f1(const char* const p)
{
    return;
}

int f1_ud(float f, int ud)
{
    return (int)(f + ud);
}

int functor1_ud(float f, void* pdata)
{
    return (int)(f + *((float*)pdata));
}

float f2(float f, int i)
{
    float asdfsa = f + i;
    return asdfsa;
}

bool f_and(bool b1, bool b2)
{
    return b1 && b2;
}

bool f_or(bool b1, bool b2)
{
    return b1 || b2;
}

class IO_CmdMy: public dconsole::BaseCmd {
public:
    IO_CmdMy(const MyString& name, const MyString desc):
        BaseCmd(name, desc) {  }

    bool runCommand(const std::vector<dconsole::BaseCmd::Variant>& params, dconsole::Console& console)
    {
        MyString str;
        for(size_t i=0; i<params.size();++i)
        {
            str = "CustomCommand: ";
            MyString pi = params[i];
            str.append(pi);
            console.addLogRecord(dconsole::Console::COL_MSG, str);
        }
        return true;
    }
protected:
    const MyString getUsage() const { return getName(); }
private:
    bool checkArguments(const std::vector<dconsole::BaseCmd::Variant>& params)
    {
        return true;
    }
};

MyString f2_str(const char* c1, const char* c2, int ud)
{
    char buf[128] = {0};
    if(strlen(c1) + strlen(c2) < 128-1 ) // one for #
    {
        if(ud)
        {
            strcpy(buf, c1);
            strcat(buf, "#");
            strcat(buf, c2);
        }
        else
        {
            strcpy(buf, c2);
            strcat(buf, "#");
            strcat(buf, c1);
        }
    }

    return MyString(&buf[0]);
}

void test() {

//    bool v1 = TypeTraits<int>::isFundamental;
//    bool v2 = TypeTraits<unsigned int>::isFundamental;
    //---------------------------------------------------------------------------
    std::vector<dconsole::BaseCmd::Variant> params;
    //params.push_back(dconsole::BaseCmd::Variant("1.86"));
    //params.push_back(dconsole::BaseCmd::Variant("5"));


    { // IO_Cmd
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd<MyString>("helloWorld", "Hello, World!", f_hw);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

	{ // IO_Cmd<void>
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd<void>("do_somethig", "void ret val test", f_voidretval);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

	{ // IO_Cmd<int>
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd<int>("ret_int", "int ret val test", f_intretval);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

    { // IO_Cmd1
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd1<int, float>("fasimple", "adder", f);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);

    pcmd = new dconsole::IO_Cmd1<void, const char*>("fachar", "adder", f1);
    pcmd ->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd );
    }

	{ // IO_Cmd2
	params.push_back(dconsole::BaseCmd::Variant("1.86"));
    params.push_back(dconsole::BaseCmd::Variant("5"));
	// rv: MyString; p1: int ; p2:int
	dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd2<MyString, int, int >("mystring_intint", "returns MyString", f_string_p1Intp2Int);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );

	pcmd = new dconsole::IO_Cmd2<const char*, int, int >("constcharptr_intint", "returns const char*", f_constcharptr_p1Intp2Int);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );

	pcmd = new dconsole::IO_Cmd2<char*, int, int >("charptr_intint", "returns chasr*", f_charptr_p1Intp2Int);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );
	// void (*fp)(int,int)
	pcmd = new dconsole::IO_Cmd2<void, int, int >("void_intint", "returns void", f_void_intint);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );
	}

    { // IO_CmdUD
    typedef int (*f1_t)(int*);
    int* pi = new int;
    *pi = 10;
    dconsole::BaseCmd* pcmd = new dconsole::IO_CmdUD<int, int*>("fint_ptr", "desc1", f_intptr, pi);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

    { // CmdVar
    dconsole::BaseCmd* pcmd = dconsole::createVariable<int>("testVar", "just a variable for tests", setTestVar, getTestVar);
    dconsole::Instance().registerCommand(pcmd);

    static int my_var = 12;
    pcmd = dconsole::createVariableUP("myVar", "just my variable for tests", &my_var);
    dconsole::Instance().registerCommand(pcmd);
    }

    { // IO_Cmd2
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd2<bool, bool, bool>("AND", "logical AND", f_and);
    dconsole::Instance().registerCommand(pcmd);
        
    pcmd = new dconsole::IO_Cmd2<bool, bool, bool>("OR", "logical OR", f_or);
    dconsole::Instance().registerCommand(pcmd);
    
    pcmd = new dconsole::IO_Cmd2<float, int, float>("int_float", "desc1", f_intfloat);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
     
    pcmd = new dconsole::IO_Cmd2<float, float, int>("func_add", "adder", f2);
    pcmd ->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);

    pcmd = new dconsole::IO_Cmd2<bool, unsigned int, bool>("load_world", "Loads world: first parameter - key, second - asyns/sync load", load_world);
    dconsole::Instance().registerCommand(pcmd );
    }

    { // IO_Cmd1UD
    typedef float (*f1_t)(int, float);
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd1UD<float, int, float>("int_floatUD", "desc1", f_intfloat, 2.35f);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

    { // IO_Cmd3
    typedef int (*f_intintint_t)(int i1, int i2, int i3);
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd3<int, int, int, int>("int3", "desc1", f_intintint);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }
    
    { // IO_Cmd2UD
    typedef MyString (*f_intfloatUD_t)(int i1, float f2, int i3);
    dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd2UD<MyString, int, float, int>("int2UD", "desc1", f_intfloatUD, 2);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

    { // Custon Command 
    dconsole::BaseCmd* pcmd = new IO_CmdMy("My_custom", "description");
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }
    
}

}

/*

////////////////////////////////////////////////////////////////////
// Debug Console
////////////////////////////////////////////////////////////////////

// Users manual
// ------------
// Intro 
// -----
// Console consists of three main parts:
// 1) command line itself
// 2) log lines
// 3) command match list
// Command Line - where you type the commands
// Log Lines - where you can see result of the command beeing executed
// Command Match List - list of commands that match your current input
// You may start entering your command from '*' then all possible commands that have typed substing in it will be added into command match list
// e.g. >*dmin will match: admin, dminimum.
//
// Command entering
// ----------------
// Same as in ordinary command line: enter command and its parameters (if any) separated by SPACEs, followed by ENTER
// If command is wrong, corresponding log record will be added along with correct usage of the command
// to get the command description use "help cmdname" command, where "cmdname" - is the name of command which description you want to get.
// You may also do "help help" :-)
// If you start typing with astesisk '*' character then all commands that contain typed substring will be in command match list; 
//	
// Parameters passing 
// ------------------
// parameters are separated by SPACE sybmols. Numbers are entered like: 1, 2.35, 0.38. Bool values as: true, false, TRuE, False
//
// Return values
// -------------
// command return value printed in the log immediately after the command itself.
//
// Variables
// ---------
// To set variable type variable name and pass its value as parameter. To ge value of a variable - just print its name, following return key.
//
// Echo
// ----
// You may turn echo on/off by setting corressponding boolean variable, e.g. "echo true/false".

// Keys
// ----
// Open/Close console - TAB
// Ctrl + SPACE BAR  - to complete the word. If nothing to complete, then current line from log will be copied to command line.
// UP/DOWN - scroll command match list by line. If no commands in command match list, then scroll among entered commands wil be performed.  
// Ctrl + UP/DOWN - scroll the log by line
// LEFT/RIGHT/BACKSPACE - navigation/edition in the command line
// PAGE_UP/PAGE_DOWN - scroll in the command match list by page
// Ctrl + PAGE_UP/PAGE_DOWN - scroll in the log by page

// Colouring
// ---------
//
// To set your own colour add next lines to "default.cfg" file:
// Console_col_msg=0xffFFFFFF
// Console_col_grayed_msg=0xff555555
// Console_col_bad_cmd=0xffFF0000
// Console_col_bad_args=0xff800080
// Console_col_cmd_result=0xff0000FF
// Console_col_echo=0xff00FF00
// Console_col_bg=0xff000000
// Console_col_info=0xffAA5500
// Console_col_params=0xffffffff
// If line is not found default colour will be assigned. You can see default colours just above



// Programmers Manual
// ------------------

// 1) IO_Console
// -------------
// It is actually a console class itself. Singleton.
// Access: 
	IO_Console& console = IO_Console::Instance();
//
// 2) BaseCmd
// ------------------
// Base class for command creation. You may derive your own class from this one to create your speciall command.
// But.. this is not always needed. Why? For this see the next section.
// To be able to construct commands include "IO_Console_Command.h" header, you may also need "IO_Console.h" header 
// 
// 3) Command construction using predefined classes
// ------------------------------------------------
//
// Using predefined classes you benefit from automatic arguments checking and converting, errors handling, usage string construction, etc..
// All that you have to do is provide c++ function or Gear Functor for your command.
//
// 3.1)Possible function parameters
// --------------------------------
//
// int, float, bool, const char* , MyString - actually you cannot enter anything else from command line, can you? :-)
// This also applies to return value, for return value you may use void also. It will be printed to console
// But user data can have any possible tye you wish (except the ones which are prohibited by  c++ like void)

// 3.2) Using function pointers
// ----------------------------
//
// There are several classes which falilitate creation of commands from function pointers 
// Suppose you have a function
	int my_func(float f, int ud)
	{
	    return (int)(f + ud);
	}
// and you want to be able to use it as a command you may use one of the provided classes.
// There are several classes:
// IO_Cmd	- no parameters, no user data  
// IO_CmdUD	- no parameters, +user data
// IO_Cmd1	- 1 parameter, no user data  
// IO_Cmd1UD	- 1 parameter, +user data
// IO_Cmd2	- 2 parameters, no user data  
// IO_Cmd2UD	- 2 parameters, +user data
// IO_Cmd3	- 3 parameters, no user data
// How do you think, which one of those classes you should use? 
// For this particular function you should use IO_Cmd2. BUt thi sis obvious
// So..
	dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd2<int, float, int>("add", "Adder", my_func);
	dconsole::Instance().registerCommand(pcmd);
// as you might noted from example, those classes are actually template classes, so you have to speialize them 
// First parameter is always a return type, and others are function parameters
// If you use function in which last parameter should be user data it is the same process. E.g.
	dconsole::BaseCmd* pcmd = new dconsole::IO_Cmd1UD<int, float, int>("add5", "Adds vive to entered value", my_func, 777);
// Now you have a command with one float parameter and integer user data (777). So when my_func is executed second parameter will be 777
// For the full list of examples see Appendix 1.
//
// 3.3) Using Gear functors
// ------------------------
//
// Using Gear functors is more robust way to create your commands, because you are not bounded by only function pointers, 
// but may also use functors to call member functions, etc. For information on how to use Gear functors see corresponding documentation.
// Here i will show how to create command using specially provided functions
// Suppose you have a function with 1 parameter + user data
	int functor1_ud(float f, void* pdata)
	{
	    return (int)(f + *((float*)pdata));
	}
	// create our functor
	typedef int (*fp_t)(float, void*);
	Gear::FunctionTranslator2<int, float, void*, fp_t> functor = Gear::MakeFunctor((Gear::Functor2<int, float, void*>*)0, functor1_ud);
	static float fvalue = 3.45f; // user data
	dconsole::BaseCmd* functor_cmd = dconsole::createFunctorCmd1UD<int>("name", "desc", functor, (void*)&fvalue);
	dconsole::Instance().registerCommand(functor_cmd);
// As you can see, nothing hard. You stil have to pass return value even if you already provided it wjen constructing functor
// But as far as there is no typedef for it inside Functor class i cannot know it. Probably this will be corrected in future by Gear team.
//
// Here is the complete list of provided functions
// - no parameters, no user data  
// createFunctorCmd0	- no parameters, +user data
// createFunctorCmd1	- 1 parameter, no user data  
// createFunctorCmd1UD	- 1 parameter, +user data
// createFunctorCmd2	- 2 parameters, no user data  
// createFunctorCmd2UD	- 2 parameters, +user data
// createFunctorCmd3	- 3 parameters, no user data
// For the full list of examples see Appendix 1.
// 
// 4) Custom commands construction.
// --------------------------------
// 
// Sometimes there might be a situation when you need to make some custom processing. For this you may derive a class from IO_Base_Command.
// In this case of course you will need to check (and convert) your arguments by yourself.
// Here is an example of how you can do it:
//
class IO_CmdMy: public dconsole::BaseCmd {
public:
    IO_CmdMy(const MyString& name, const MyString desc):
        BaseCmd(name, desc) {  }

    bool runCommand(const std::vector<dconsole::BaseCmd::Variant>& params, dconsole::Console& console)
    {
        MyString str;
        for(size_t i=0; i<params.size();++i)
        {
            str = "CustomCommand: ";
            MyString pi = params[i];
            str.append(pi);
            console.addLogRecord(dconsole::Console::COL_MSG, str);
        }
        return true;
    }
protected:
    const MyString getUsage() const { return getName(); }
private:
    bool checkArguments(const std::vector<dconsole::BaseCmd::Variant>& params)
    {
        return true;
    }
};

// 5) Adding variables 
// ------------------- 
//
// Adding using set/get pair
// -------------------------
// To add a variable one needs to provide set/get pair and its name. It can be done using special template class CmdVar
// For example we want to create int variable named testVar
	dconsole::BaseCmd* pcmd = new dconsole::createVariable<int>("testVar", "just a variable for tests", setTestVar, getTestVar);
	dconsole::Instance().registerCommand(pcmd);
// as you can see you have to provide set and get functions. Signature should be as follow:
	typedef void (*set_fp)(T v)
	typedef T (*get_fp)(void)
// where T - is the type of your variable you wish to be
//
// Adding using variable pointer
// -----------------------------
// To create console variable using pointer just provide it to corresponding creation function CmdVarUP
// e.g.
	static int my_var = 12;
	dconsole::BaseCmd* pcmd = dconsole::createVariableUP("myVar", "just my variable for tests", &my_var);
	dconsole::Instance().registerCommand(pcmd);
// That is it! Do not forget that console does not do memory management for your variables.


*/