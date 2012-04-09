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

    //---------------------------------------------------------------------------
    std::vector<dconsole::BaseCmd::Variant> params;

    { // 
    dconsole::BaseCmd* pcmd = createCmd("helloWorld", "Hello, World!", f_hw);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

	{ // 
    dconsole::BaseCmd* pcmd = createCmd("do_somethig", "void ret val test", f_voidretval);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

	{ // >
    dconsole::BaseCmd* pcmd = createCmd("ret_int", "int ret val test", f_intretval);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }

    { // 
    dconsole::BaseCmd* pcmd = createCmd("fasimple", "adder", f);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);

    pcmd = createCmd("fachar", "adder", f1);
    pcmd ->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd );
    }

	{ // 
	params.push_back(dconsole::BaseCmd::Variant("1.86"));
    params.push_back(dconsole::BaseCmd::Variant("5"));
	// rv: MyString; p1: int ; p2:int
	dconsole::BaseCmd* pcmd = createCmd("mystring_intint", "returns MyString", f_string_p1Intp2Int);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );

	pcmd = createCmd("constcharptr_intint", "returns const char*", f_constcharptr_p1Intp2Int);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );

	pcmd = createCmd("charptr_intint", "returns chasr*", f_charptr_p1Intp2Int);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );
	// void (*fp)(int,int)
	pcmd = createCmd("void_intint", "returns void", f_void_intint);
	pcmd ->runCommand(params, dconsole::Instance());
	dconsole::Instance().registerCommand(pcmd );
	}

    { // 
    dconsole::BaseCmd* pcmd = createCmd("AND", "logical AND", f_and);
    dconsole::Instance().registerCommand(pcmd);
        
    pcmd = createCmd("OR", "logical OR", f_or);
    dconsole::Instance().registerCommand(pcmd);
    
    pcmd = createCmd("int_float", "desc1", f_intfloat);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
     
    pcmd = createCmd("func_add", "adder", f2);
    pcmd ->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);

    pcmd = createCmd("load_world", "Loads world: first parameter - key, second - asyns/sync load", load_world);
    dconsole::Instance().registerCommand(pcmd );
    }

    { // 
    typedef int (*f_intintint_t)(int i1, int i2, int i3);
    dconsole::BaseCmd* pcmd = createCmd("int3", "desc1", f_intintint);
    pcmd->runCommand(params, dconsole::Instance());
    dconsole::Instance().registerCommand(pcmd);
    }
}

}
