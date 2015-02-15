/**
 *  抄自qt，定义可组合的flag
 *
 *  @author zhangyi
 *  @since  2012-08-20
 */

#ifndef SERVER3_BASE_UTILS_FLAGS_H
#define SERVER3_BASE_UTILS_FLAGS_H

class CFlag
{
    int _i;
public:
    inline CFlag(int i) : _i(i) {}
    inline operator int() const { return _i; }
};

class CIncompatibleFlag
{
    int i;
public:
    inline explicit CIncompatibleFlag(int i){this->i = i;};
    inline operator int() const { return i; }
};

template<typename Enum>
class CFlags
{
    typedef void **Zero;
    int i;
public:
    typedef Enum enum_type;
    inline CFlags(const CFlags &f) : i(f.i) {}
    inline CFlags(Enum f) : i(f) {}
    inline CFlags(Zero = 0) : i(0) {}
    inline CFlags(CFlag f) : i(f) {}

    inline CFlags &operator=(const CFlags &f) { i = f.i; return *this; }
    inline CFlags &operator&=(int mask) { i &= mask; return *this; }
    inline CFlags &operator|=(CFlags f) { i |= f.i; return *this; }
    inline CFlags &operator|=(Enum f) { i |= f; return *this; }
    inline CFlags &operator^=(CFlags f) { i ^= f.i; return *this; }
    inline CFlags &operator^=(Enum f) { i ^= f; return *this; }

     inline operator int() const { return i; }

    inline CFlags operator|(CFlags f) const { return CFlags(Enum(i | f.i)); }
    inline CFlags operator|(Enum f) const { return CFlags(Enum(i | f)); }
    inline CFlags operator^(CFlags f) const { return CFlags(Enum(i ^ f.i)); }
    inline CFlags operator^(Enum f) const { return CFlags(Enum(i ^ f)); }
    inline CFlags operator&(int mask) const { return CFlags(Enum(i & mask)); }
    inline CFlags operator&(Enum f) const { return CFlags(Enum(i & f)); }
    inline CFlags operator~() const { return CFlags(Enum(~i)); }

    inline bool operator!() const { return !i; }

    inline bool testFlag(Enum f) const { return (i & f) == f && (f != 0 || i == int(f) ); }
};

#define TT_DECLARE_FLAGS(Flags, Enum)\
    typedef CFlags<Enum> Flags;

#define TT_DECLARE_INCOMPATIBLE_FLAGS(Flags) \
    inline CIncompatibleFlag operator|(Flags::enum_type f1, int f2) \
{ return CIncompatibleFlag(int(f1) | f2); }

#define TT_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
    inline CFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) \
{ return CFlags<Flags::enum_type>(f1) | f2; } \
    inline CFlags<Flags::enum_type> operator|(Flags::enum_type f1, CFlags<Flags::enum_type> f2) \
{ return f2 | f1; } TT_DECLARE_INCOMPATIBLE_FLAGS(Flags)

#endif
