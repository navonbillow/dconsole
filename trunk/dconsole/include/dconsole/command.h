#ifndef __IO_CONSOLE_COMMAND_H__
#define __IO_CONSOLE_COMMAND_H__

/* 
 * Console implementation example.
 * alariq@gmail.com
 * Distributed under terms of the LGPL.
 */

#include <strstream>   // for sstream
#include <sstream>
#include <vector>

#include "dconsole/base_command.h"

namespace dconsole {

//sorry for that
#define ADD_COMMON(t) template<typename U>    struct is_ ## t        { enum { value = false }; };      
#define ADD_ENUM(t)   enum { e_is_ ## t = is_ ## t <T>::value };
#define ADD_SPEC(t)   template<>              struct BaseTypeTraits::is_ ## t < t >   { enum { value = true };  }; 

#define ADD_COMMON2(qualifier, t) template<typename U>    struct is_ ## qualifier ## t        { enum { value = false }; };
#define ADD_ENUM2(qualifier, t)   enum { e_is_ ## qualifier ## t = is_ ## qualifier ## t <T>::value };
#define ADD_SPEC2(qualifier, t)   template<>              struct BaseTypeTraits::is_ ## qualifier ## t < qualifier t  >   { enum { value = true };  }; 

class BaseTypeTraits {
protected:
    ADD_COMMON(bool)
    ADD_COMMON(int)
    ADD_COMMON(char)
    ADD_COMMON(wchar_t)
    ADD_COMMON(short)
    ADD_COMMON(long)

    ADD_COMMON(float)
    ADD_COMMON(double)

    ADD_COMMON2(unsigned, int)
    ADD_COMMON2(unsigned, char)
    ADD_COMMON2(unsigned, short)
    ADD_COMMON2(unsigned, long)
};

ADD_SPEC(bool)
ADD_SPEC(int)
ADD_SPEC(char)
ADD_SPEC(wchar_t)
ADD_SPEC(short)
ADD_SPEC(long)

ADD_SPEC(float)
ADD_SPEC(double)

ADD_SPEC2(unsigned, int)
ADD_SPEC2(unsigned, char)
ADD_SPEC2(unsigned, short)
ADD_SPEC2(unsigned, long)

template <typename T>
class TypeTraits: public BaseTypeTraits {

    ADD_ENUM(bool)
    ADD_ENUM(int)
    ADD_ENUM(char)
    ADD_ENUM(wchar_t)
    ADD_ENUM(short)
    ADD_ENUM(long)

    ADD_ENUM(float)
    ADD_ENUM(double)

    ADD_ENUM2(unsigned, int)
    ADD_ENUM2(unsigned, char)
    ADD_ENUM2(unsigned, short)
    ADD_ENUM2(unsigned, long)

public:
    enum { isSignedIntegral = is_bool<T>::value || is_int<T>::value || is_char<T>::value || is_wchar_t<T>::value || is_short<T>::value || is_long<T>::value };
    enum { isUnignedIntegral = is_unsignedint<T>::value || is_unsignedchar<T>::value || is_unsignedshort<T>::value || is_unsignedlong<T>::value };

    enum { isIntegral = isSignedIntegral || isUnignedIntegral }; 
    enum { isFundamental = isIntegral || is_float<T>::value || is_double<T>::value };
};

#undef ADD_COMMON
#undef ADD_ENUM
#undef ADD_SPEC

#undef ADD_COMMON2
#undef ADD_ENUM2
#undef DEF_SPEC2


// TypeList class
//==========================================================================================
class NullType {};

template <class T, class U> 
struct TypeList {
	typedef T Head;
	typedef U Tail;
};
// get index of type in a list
// ===========================
template < class TList, class T> struct  indexOf;
template <class T>
struct indexOf<NullType, T> {
	enum { value  = -1 };
};

template <class T, class Tail>
struct indexOf<TypeList<T, Tail>, T > {
	enum { value  = 0 };
};

template <class Head, class Tail, class T>
struct indexOf<TypeList<Head, Tail>, T > {
private:
	enum { temp = indexOf<Tail, T>::value };
public:
	enum { value  = temp == -1 ? -1 : 1 + temp };
};

// end of TypeList class ======================================================================

// to distinguish types that can easily be converted to MyString
typedef TypeList<MyString, TypeList<char*, TypeList<const char*, NullType> > > StringType;

// for static assertions, see below
template <bool b> struct You_can_only_use_variable_commands_with_nonconst_fundamental_types;
template<> struct You_can_only_use_variable_commands_with_nonconst_fundamental_types<true> {};


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// doCmd classes realisation for different number of parameters, used in IO_Cmd* classes
////////////////////////////////////// doCmd0 ///////////////////////////////////////////////////////////////////
template <typename Cmd_t, typename rv, bool isStringType> class doCmd0;

// rv: void
template<typename Cmd_t> class doCmd0<Cmd_t, void, indexOf<StringType, void>::value!=-1> {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, Console& console) {
        func_ptr();
    }
};
// rv: strings
template<typename Cmd_t, typename rv> class doCmd0<Cmd_t, rv, true>
{
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, class Console& console) {
        MyString str = func_ptr();
        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};
// rv: not trings
template<typename Cmd_t, typename rv> class doCmd0<Cmd_t, rv, false>
{
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, class Console& console) {
		rv retval = func_ptr();

		MyString str;
		std::stringstream inout;
		inout<<retval;
		inout>>str;

        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};
/////////////////////////////////// doCmd1 //////////////////////////////////////////////////////////////////////
template <typename Cmd_t, typename rv, bool isStringType> class doCmd1;

// rv: void
template<typename Cmd_t> class doCmd1<Cmd_t, void, indexOf<StringType, void>::value!=-1 > {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, Console& console) {
        func_ptr(param1);
    }
};
// rv: strings
template <typename Cmd_t, typename rv> class doCmd1<Cmd_t, rv, true> {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, class Console& console) {
        MyString str = func_ptr(param1); 
        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};
// rv: not strings
template <typename Cmd_t, typename rv> class doCmd1<Cmd_t, rv, false> {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, class Console& console) {
        rv retval = func_ptr(param1); 
        MyString str;
		std::stringstream inout;
		inout<<retval;
		inout>>str;
        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};
/////////////////////////////////// doCmd2 //////////////////////////////////////////////////////////////////////
template <typename Cmd_t, typename rv, bool isStringType> class doCmd2;

// rv: void
template<typename Cmd_t> class doCmd2<Cmd_t, void, indexOf<StringType, void>::value!=-1 > {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, const typename Cmd_t::secondParam_t param2, class Console& console) {
        func_ptr(param1, param2);
    }
};
// rv: strings
template <typename Cmd_t, typename rv> class doCmd2<Cmd_t, rv, true> {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, const typename Cmd_t::secondParam_t param2, class Console& console) {
        MyString str = func_ptr(param1, param2); 
        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};
// rv: not strings
template <typename Cmd_t, typename rv> class doCmd2<Cmd_t, rv, false> {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, const typename Cmd_t::secondParam_t param2, class Console& console) {
        rv retval = func_ptr(param1, param2);
        MyString str;
		std::stringstream inout;
		inout<<retval;
		inout>>str;
        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};

/////////////////////////////////////// doCmd3 //////////////////////////////////////////////////////////////////
template <typename Cmd_t, typename rv, bool isStringType > class doCmd3;

// rv: void
template<typename Cmd_t> class doCmd3<Cmd_t, void, indexOf<StringType, void>::value!=-1>
{
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, const typename Cmd_t::secondParam_t param2, const typename Cmd_t::thirdParam_t param3, class Console& console) {
        func_ptr(param1, param2, param3);
    }
};
// rv: strings
template <typename Cmd_t, typename rv> class doCmd3<Cmd_t, rv, true> {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, const typename Cmd_t::secondParam_t param2, const typename Cmd_t::thirdParam_t param3, class Console& console) {
        MyString str = func_ptr(param1, param2, param3); 
        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};
// rv: not strings
template <typename Cmd_t, typename rv> class doCmd3<Cmd_t, rv, false> {
public:
    static void doCommand(typename Cmd_t::FP_t func_ptr, const typename Cmd_t::firstParam_t param1, const typename Cmd_t::secondParam_t param2, const typename Cmd_t::thirdParam_t param3, class Console& console) {
        rv retval = func_ptr(param1, param2, param3);
        MyString str;
		std::stringstream inout;
		inout<<retval;
		inout>>str;
        console.addLogRecord(Console::COL_CMD_RESULT, str);
    }
};


//===================  IO_Cmd  ====================================================================================
//=================== no params at all ============================================================================
template <typename R>
class IO_Cmd: public BaseCmd {
    typedef IO_Cmd<R> Me_t;
    friend class dconsole::doCmd0<Me_t, R, indexOf<StringType,R>::value!=-1>;
protected:
    typedef R returnValue_t;
    typedef R (*FP_t)(void);
    FP_t m_func_ptr;
public:
    IO_Cmd(const MyString& name, const MyString desc, FP_t fp):
        BaseCmd(name, desc), m_func_ptr(fp) {  }

    bool runCommand(const std::vector<BaseCmd::Variant>& params, Console& console)
    {
        if( !checkArguments(params) )
            return false;
        dconsole::doCmd0<Me_t, R, indexOf<StringType,R>::value!=-1 >::doCommand(m_func_ptr, console);
        return true;
    }
protected:
    const MyString getUsage() const { return getName(); }
private:
    bool checkArguments(const std::vector<BaseCmd::Variant>& params)
    {
        return params.size() == 0;
    }
};
//========================  IO_Cmd1  ==============================================================================
//=================================================================================================================
template <typename R, typename T>
class IO_Cmd1: public BaseCmd {
    typedef IO_Cmd1<R,T> Me_t;
    friend class dconsole::doCmd1<Me_t, R, indexOf<StringType,R>::value!=-1 >;
protected:
    typedef R (*FP_t)(T);
    typedef T firstParam_t;
    typedef R returnValue_t;  

    FP_t m_func_ptr;
public:
    IO_Cmd1(const MyString& name, const MyString desc, FP_t fp):
    BaseCmd(name, desc)
    ,m_func_ptr(fp)
    {
    }
    bool runCommand(const std::vector<BaseCmd::Variant>& params, Console& console)
    {
        if( !checkArguments(params) )
            return false;
        dconsole::doCmd1<Me_t, R, indexOf<StringType,R>::value!=-1 >::doCommand(m_func_ptr, params[0], console);
        return true;
    }
protected:
    const MyString getUsage() const
    {
        MyString str;
        MyString p1 = Type2String<T>::name();
        str.append(getName());str.append(" ");
        str.append( p1 );
        return str;
    }
private:
    bool checkArguments(const std::vector<BaseCmd::Variant>& params)
    {
        if(params.size() != 1)
            return false;
        if( !params[0].is<T>() )
            return false;
        return true;
    }
};

//========================  IO_CmdUD  =============================================================================
//================== no parameters, but 1 userdata value ==========================================================
template <typename R, typename UD> 
class IO_CmdUD: public BaseCmd {
    typedef IO_CmdUD<R, UD> Me_t;
    friend class dconsole::doCmd1<Me_t, R, indexOf<StringType,R>::value!=-1 >;
protected:
    typedef R (*FP_t)(UD);
    typedef R returnValue_t;
    typedef UD firstParam_t; // for correct work of doCmd
    typedef UD UserData_t;

    FP_t m_func_ptr;
    UserData_t m_userValue;
public:
    IO_CmdUD(const MyString& name, const MyString desc, FP_t fp, UserData_t userValue):
    BaseCmd(name, desc)
    , m_userValue(userValue),m_func_ptr(fp)
    {
    }
    bool runCommand(const std::vector<BaseCmd::Variant>& params, Console& console)
    {
        if( !checkArguments(params) )
            return false;
        dconsole::doCmd1<Me_t, R, indexOf<StringType,R>::value!=-1 >::doCommand(m_func_ptr, m_userValue, console);
        return true;
    }
protected:
    const MyString getUsage() const { return getName(); }
private:
    bool checkArguments(const std::vector<BaseCmd::Variant>& params)
    {
        if(params.size() > 0)
            return false;
        return true;
    }
};

//========================  IO_Cmd2  ==============================================================================
//=================================================================================================================
template <typename R, typename T1, typename T2>
class IO_Cmd2: public BaseCmd {
    typedef IO_Cmd2<R,T1, T2> Me_t;
	friend class dconsole::doCmd2<Me_t, R, indexOf<StringType,R>::value!=-1 >;
protected:
    typedef R (*FP_t)(T1, T2);
    typedef T1 firstParam_t;
    typedef T2 secondParam_t;
    typedef R returnValue_t;

    FP_t m_func_ptr;
public:
    IO_Cmd2(const MyString& name, const MyString desc, FP_t fp):
    BaseCmd(name, desc)
    ,m_func_ptr(fp)
    {
    }
    bool runCommand(const std::vector<BaseCmd::Variant>& params, Console& console)
    {
        if( !checkArguments(params) )
            return false;
		dconsole::doCmd2<Me_t, R, indexOf<StringType,R>::value !=-1 >::doCommand(m_func_ptr, params[0], params[1], console);
        return true;
    }
protected:
    const MyString getUsage() const
    {
        MyString str;
        MyString p1 = Type2String<T1>::name();
        MyString p2 = Type2String<T2>::name();
        str.append(getName());str.append(" ");
        str.append( p1 );str.append(" ");
        str.append( p2 );
        return str;
    }
private:
    bool checkArguments(const std::vector<BaseCmd::Variant>& params)
    {
        if(params.size() != 2)
            return false;
        if( !params[0].is<T1>() || !params[1].is<T2>())
            return false;
        return true;
    }
};

//========================  IO_Cmd1UD  ==============================================================================
//=================================================================================================================
template <typename R, typename T1, typename UD>
class IO_Cmd1UD: public BaseCmd {
    typedef IO_Cmd1UD<R,T1, UD> Me_t;
    friend class dconsole::doCmd2<Me_t, R, indexOf<StringType,R>::value!=-1 >;
protected:
    typedef R (*FP_t)(T1, UD);
    typedef R returnValue_t;
    typedef T1 firstParam_t;
    typedef UD secondParam_t;
    typedef UD UserData_t;

    FP_t m_func_ptr;
    UserData_t m_userValue;
public:
    IO_Cmd1UD(const MyString& name, const MyString desc, FP_t fp, UserData_t userValue):
        BaseCmd(name, desc), m_func_ptr(fp), m_userValue(userValue) {}

    bool runCommand(const std::vector<BaseCmd::Variant>& params, Console& console)
    {
        if( !checkArguments(params) )
            return false;
        dconsole::doCmd2<Me_t, R, indexOf<StringType,R>::value!=-1 >::doCommand(m_func_ptr, params[0], m_userValue, console);
        return true;
    }
protected:
    const MyString getUsage() const
    {
        MyString str;
        MyString p1 = Type2String<T1>::name();
        str.append(getName());str.append(" ");
        str.append( p1 );
        return str;
    }
private:
    bool checkArguments(const std::vector<BaseCmd::Variant>& params)
    {
        if(params.size() != 1)
            return false;
        if( !params[0].is<T1>())
            return false;
        return true;
    }
};

//========================  IO_Cmd3  ==============================================================================
//=================================================================================================================
template <typename R, typename T1, typename T2, typename T3>
class IO_Cmd3: public BaseCmd {
    typedef IO_Cmd3<R,T1, T2, T3> Me_t;
	friend class dconsole::doCmd3<Me_t, R, indexOf<StringType, R>::value!=-1 >;
protected:
    typedef R (*FP_t)(T1, T2, T3);
    typedef T1 firstParam_t;
    typedef T2 secondParam_t;
    typedef T3 thirdParam_t;
    typedef R returnValue_t;  

    FP_t m_func_ptr;
public:
    IO_Cmd3(const MyString& name, const MyString desc, FP_t fp):
    BaseCmd(name, desc)
    ,m_func_ptr(fp)
    {
    }
    bool runCommand(const std::vector<BaseCmd::Variant>& params, Console& console)
    {
        if( !checkArguments(params) )
            return false;
        dconsole::doCmd3<Me_t, R, indexOf<StringType, R>::value!=-1 >::doCommand(m_func_ptr, params[0], params[1], params[2], console);
        return true;
    }
protected:
    const MyString getUsage() const
    {
        MyString str;
        MyString p1 = Type2String<T1>::name();
        MyString p2 = Type2String<T2>::name();
        MyString p3 = Type2String<T3>::name();
        str.append(getName());str.append(" ");
        str.append( p1 );str.append(" ");
        str.append( p2 );str.append(" ");
        str.append( p3 );
        return str;
    }
private:
    bool checkArguments(const std::vector<BaseCmd::Variant>& params)
    {
        if(params.size() != 3)
            return false;
        if( !params[0].is<T1>() || !params[1].is<T2>() || !params[2].is<T3>())
            return false;
        return true;
    }
};

//========================  IO_Cmd2UD  ==============================================================================
//=================================================================================================================
template <typename R, typename T1, typename T2, typename UD>
class IO_Cmd2UD: public BaseCmd {
    typedef IO_Cmd2UD<R,T1, T2, UD> Me_t;
    friend class dconsole::doCmd3<Me_t, R, indexOf<StringType, R>::value!=-1>;
protected:
    typedef R (*FP_t)(T1, T2, UD);
    typedef R returnValue_t;
    typedef T1 firstParam_t;
    typedef T2 secondParam_t;
    typedef UD thirdParam_t;
    typedef UD UserData_t;

    FP_t m_func_ptr;
    UserData_t m_userValue;
public:
    IO_Cmd2UD(const MyString& name, const MyString desc, FP_t fp, UserData_t userValue):
        BaseCmd(name, desc), m_func_ptr(fp), m_userValue(userValue) {}

    bool runCommand(const std::vector<BaseCmd::Variant>& params, Console& console)
    {
        if( !checkArguments(params) )
            return false;
        dconsole::doCmd3<Me_t, R, indexOf<StringType, R>::value!=-1 >::doCommand(m_func_ptr, params[0], params[1], m_userValue, console);
        return true;
    }
protected:
    const MyString getUsage() const
    {
        MyString str;
        MyString p1 = Type2String<T1>::name();
        MyString p2 = Type2String<T2>::name();
        str.append(getName());str.append(" ");
        str.append( p1 );str.append(" ");
        str.append( p2 );
        return str;
    }
private:
    bool checkArguments(const std::vector<BaseCmd::Variant>& params)
    {
        if(params.size() != 2)
            return false;
        if( !params[0].is<T1>() || !params[1].is<T2>())
            return false;
        return true;
    }
};
//=================================================================================================================

//============================ IO_CmdVar ==========================================================
//=================================================================================================

template <typename VarType> class CmdVar;

template <typename VarType>
class CmdVar: public dconsole::BaseCmd {
public:
    class Setter {
    public:
            typedef VarType firstParam_t;
            typedef void    returnValue_t;
            typedef void    (*FP_t)(VarType);
    };

    class Getter {
    public:
            typedef void    firstParam_t;
            typedef VarType returnValue_t;
            typedef VarType (*FP_t)(void);
    };
protected:
    friend class doCmd1<Setter, typename Setter::returnValue_t, indexOf<StringType, typename Setter::returnValue_t >::value!=-1 >;
    friend class doCmd0<Getter, typename Getter::returnValue_t, indexOf<StringType, typename Setter::returnValue_t >::value!=-1 >;

    typename Setter::FP_t    m_fpSet;
    typename Getter::FP_t    m_fpGet;
public:
    CmdVar(const MyString& name, const MyString desc,  typename Setter::FP_t fpsetter, typename Getter::FP_t fpgetter):
        BaseCmd(name, desc), m_fpSet(fpsetter), m_fpGet(fpgetter)
    { 
        You_can_only_use_variable_commands_with_nonconst_fundamental_types< TypeTraits<VarType>::isFundamental >();
    }

    bool runCommand(const std::vector<dconsole::BaseCmd::Variant>& params, dconsole::Console& console)
    {

         if( !checkArguments(params) )
            return false;

        // we already know that if we have parameter - it is only one and of type UD
        //if(params.size()) // set 
        //{
        //    doCmd1<Setter, typename Setter::returnValue_t>::doCommand(m_fpSet, params[0], console);
        //}
        //else
        //{
        //    doCmd0<Getter, typename Getter::returnValue_t>::doCommand(m_fpGet, console);
        //}

        // God sees i did not wanted to do tis way...
        if(params.size()) // set 
        {
            m_fpSet(params[0]);
        }

        VarType rv = m_fpGet();

        MyString c_name = m_name;
        c_name.append(" ");

        MyString value;

		std::stringstream inout;
		inout<<rv;
		inout>>value;

        Console::LogRecord r;
        r.push_back( Console::LogPair(console.logColor(Console::COL_CMD_RESULT), c_name) );
        r.push_back( Console::LogPair(console.logColor(Console::COL_MSG), value) );
        console.addLogRecord(r);
        //-------------------------------------------------

        return true;
    }
protected:
    const MyString getUsage() const
    {
        MyString str;
        MyString p1 = Type2String<VarType>::name();
        str.append(getName());str.append(" [ ");
        str.append( p1 );str.append(" ]");
        return str;
    }
private:
    bool checkArguments(const std::vector<dconsole::BaseCmd::Variant>& params)
    {
        if(1 == params.size() && params[0].is<VarType>())
            return true;
        if(0 == params.size())
            return true;
        return false;
    }
};


//============================ IO_CmdVar ==========================================================
//=================================================================================================

//template <typename VarType, bool isFundamental = TypeTraits<VarType>::isFundamental > class CmdVarUP;

template <typename VarType>
class CmdVarUP: public dconsole::BaseCmd {
protected:
    VarType* const m_userPtr;
public:
    CmdVarUP(const MyString& name, const MyString desc, VarType* userPtr):
        BaseCmd(name, desc), m_userPtr(userPtr)
        { 
            You_can_only_use_variable_commands_with_nonconst_fundamental_types< TypeTraits<VarType>::isFundamental >();
        }

    bool runCommand(const std::vector<dconsole::BaseCmd::Variant>& params, dconsole::Console& console)
    {
         if( !checkArguments(params) )
            return false;
        // we already know that if we have parameter - it is only one and of type UD
        // God sees i did not wanted to do tis way...
        if(params.size()) // set 
        {
            *m_userPtr = params[0];
        }

        VarType rv = *m_userPtr;

        MyString c_name = m_name;
        c_name.append(" ");

        MyString value;

		std::stringstream inout;
		inout<<rv;
		inout>>value;

        Console::LogRecord r;
        r.push_back( Console::LogPair(console.logColor(Console::COL_CMD_RESULT), c_name) );
        r.push_back( Console::LogPair(console.logColor(Console::COL_MSG), value) );
        console.addLogRecord(r);
        //-------------------------------------------------

        return true;
    }
protected:
    const MyString getUsage() const
    {
        MyString str;
        MyString p1 = Type2String<VarType>::name();
        str.append(getName());str.append(" [ ");
        str.append( p1 );str.append(" ]");
        return str;
    }
private:
    bool checkArguments(const std::vector<dconsole::BaseCmd::Variant>& params)
    {
        if(1 == params.size() && params[0].is<VarType>())
            return true;
        if(0 == params.size())
            return true;
        return false;
    }
};


//template <typename VarType, bool isFundamental >
//class CmdVarUP<VarType, isFundamental>: public dconsole::BaseCmd {
//public:
//    CmdVarUP(const MyString& name, const MyString desc, VarType* userPtr):
//        BaseCmd(name, desc), m_userPtr(userPtr) { You_can_only_use_variable_commands_with_fundamental_datatypes<isFundamental> t;  }
//
//    bool runCommand(const std::vector<dconsole::BaseCmd::Variant>& params, dconsole::Console& console)
//    {
//        return true;
//    }
//protected:
//    const MyString getUsage() const { return MyString; }
//private:
//    bool checkArguments(const std::vector<dconsole::BaseCmd::Variant>& params) { return false; }
//};



/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions for creation of commands from Gear functors
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 console parameters
//template <typename R, typename Functor_t>
//IO_FunctorCmd0<R, Functor_t>* createFunctorCmd0(const char* name, const char* desc, Functor_t functor)
//{
//    return new IO_FunctorCmd0<R, Functor_t>(name, desc, functor);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 1 console parameter
//template <typename R, typename Functor_t>
//IO_FunctorCmd1<R, Functor_t>* createFunctorCmd1(const char* name, const char* desc, Functor_t functor)
//{
//    return new IO_FunctorCmd1<R, Functor_t>(name, desc, functor);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 1 console parameter + 1 parameter as user data
//template <typename R, typename Functor_t>
//IO_FunctorCmd1UD<R, Functor_t>* createFunctorCmd1UD(const char* name, const char* desc, Functor_t functor, typename Functor_t::SecondParamType userValue)
//{
//    return new IO_FunctorCmd1UD<R, Functor_t>(name, desc, functor, userValue);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 2 console parameters
//template <typename R, typename Functor_t>
//static IO_FunctorCmd2<R, Functor_t>* createFunctorCmd2(const char* name, const char* desc, Functor_t functor)
//{
//    return new IO_FunctorCmd2<R, Functor_t>(name, desc, functor);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 2 console parameters + 1 parameter as user data
//template <typename R, typename Functor_t>
//static IO_FunctorCmd2UD<R, Functor_t>* createFunctorCmd2UD(const char* name, const char* desc, Functor_t functor, typename Functor_t::ThirdParamType userValue)
//{
//    return new IO_FunctorCmd2UD<R, Functor_t>(name, desc, functor, userValue);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 3 console parameters
//template <typename R, typename Functor_t>
//static IO_FunctorCmd3<R, Functor_t>* createFunctorCmd3(const char* name, const char* desc, Functor_t functor)
//{
//    return new IO_FunctorCmd3<R, Functor_t>(name, desc, functor);
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// console variable creation passing get/set pair
template <typename VarType>
static CmdVar<VarType>* createVariable(const char* name, const char* desc, typename CmdVar<VarType>::Setter::FP_t setter_fp, typename CmdVar<VarType>::Getter::FP_t getter_fp)
{
    return new CmdVar<VarType>(name, desc, setter_fp, getter_fp);
}
template <typename VarType>
static CmdVar<VarType>* createVariable2(const char* name, const char* desc, typename CmdVar<VarType>::Setter::FP_t setter_fp, typename CmdVar<VarType>::Getter::FP_t getter_fp)
{
    return new CmdVar<VarType>(name, desc, setter_fp, getter_fp);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// console variable creation passing user pointer
template <typename VarType>
static CmdVarUP<VarType>* createVariableUP(const char* name, const char* desc, VarType* userPtr)
{
    return new CmdVarUP<VarType>(name, desc, userPtr);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////


void test();

}

#endif //__IO_CONSOLE_COMMAND_H__