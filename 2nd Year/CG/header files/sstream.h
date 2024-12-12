// String based streams -*- C++ -*-

// Copyright (C) 1997-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file include/sstream
 *  This is a Standard C++ Library header.
 */

//
// ISO C++ 14882: 27.7  String-based streams
//

#ifndef _GLIBCXX_SSTREAM
#define _GLIBCXX_SSTREAM 1

#pragma GCC system_header

#include <bits/requires_hosted.h> // iostream

#include <istream>
#include <ostream>
#include <bits/alloc_traits.h> // allocator_traits, __allocator_like

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
# define _GLIBCXX_LVAL_REF_QUAL &
# define _GLIBCXX_SSTREAM_ALWAYS_INLINE
#else
# define _GLIBCXX_LVAL_REF_QUAL
// For symbols that are not exported from libstdc++.so for the COW string ABI.
# define _GLIBCXX_SSTREAM_ALWAYS_INLINE [[__gnu__::__always_inline__]]
#endif



namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
_GLIBCXX_BEGIN_NAMESPACE_CXX11

  // [27.7.1] template class basic_stringbuf
  /**
   *  @brief  The actual work of input and output (for std::string).
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *  @tparam _Alloc  Allocator type, defaults to allocator<_CharT>.
   *
   *  This class associates either or both of its input and output sequences
   *  with a sequence of characters, which can be initialized from, or made
   *  available as, a @c std::basic_string.  (Paraphrased from [27.7.1]/1.)
   *
   *  For this class, open modes (of type @c ios_base::openmode) have
   *  @c in set if the input sequence can be read, and @c out set if the
   *  output sequence can be written.
  */
  template<typename _CharT, typename _Traits, typename _Alloc>
    class basic_stringbuf : public basic_streambuf<_CharT, _Traits>
    {
      struct __xfer_bufptrs;

#if __cplusplus >= 201103L
      using allocator_traits = std::allocator_traits<_Alloc>;
      using _Noexcept_swap
	= __or_<typename allocator_traits::propagate_on_container_swap,
		typename allocator_traits::is_always_equal>;
#endif

    public:
      // Types:
      typedef _CharT 					char_type;
      typedef _Traits 					traits_type;
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 251. basic_stringbuf missing allocator_type
      typedef _Alloc				       	allocator_type;
      typedef typename traits_type::int_type 		int_type;
      typedef typename traits_type::pos_type 		pos_type;
      typedef typename traits_type::off_type 		off_type;

      typedef basic_streambuf<char_type, traits_type>  	__streambuf_type;
      typedef basic_string<char_type, _Traits, _Alloc> 	__string_type;
      typedef typename __string_type::size_type		__size_type;

    protected:
      /// Place to stash in || out || in | out settings for current stringbuf.
      ios_base::openmode 	_M_mode;

      // Data Members:
      __string_type 		_M_string;

    public:
      // Constructors:

      /**
       *  @brief  Starts with an empty string buffer.
       *
       *  The default constructor initializes the parent class using its
       *  own default ctor.
      */
      basic_stringbuf()
      : __streambuf_type(), _M_mode(ios_base::in | ios_base::out), _M_string()
      { }

      /**
       *  @brief  Starts with an empty string buffer.
       *  @param  __mode  Whether the buffer can read, or write, or both.
       *
       *  The default constructor initializes the parent class using its
       *  own default ctor.
      */
      explicit
      basic_stringbuf(ios_base::openmode __mode)
      : __streambuf_type(), _M_mode(__mode), _M_string()
      { }

      /**
       *  @brief  Starts with an existing string buffer.
       *  @param  __str  A string to copy as a starting buffer.
       *  @param  __mode  Whether the buffer can read, or write, or both.
       *
       *  This constructor initializes the parent class using its
       *  own default ctor.
      */
      explicit
      basic_stringbuf(const __string_type& __str,
		      ios_base::openmode __mode = ios_base::in | ios_base::out)
      : __streambuf_type(), _M_mode(),
	_M_string(__str.data(), __str.size(), __str.get_allocator())
      { _M_stringbuf_init(__mode); }

#if __cplusplus >= 201103L
      basic_stringbuf(const basic_stringbuf&) = delete;

      basic_stringbuf(basic_stringbuf&& __rhs)
      : basic_stringbuf(std::move(__rhs), __xfer_bufptrs(__rhs, this))
      { __rhs._M_sync(const_cast<char_type*>(__rhs._M_string.data()), 0, 0); }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
      explicit
      basic_stringbuf(const allocator_type& __a)
      : basic_stringbuf(ios_base::in | std::ios_base::out, __a)
      { }

      basic_stringbuf(ios_base::openmode __mode,
		      const allocator_type& __a)
      : __streambuf_type(), _M_mode(__mode), _M_string(__a)
      { }

      explicit
      basic_stringbuf(__string_type&& __s,
		      ios_base::openmode __mode = ios_base::in
						  | ios_base::out)
      : __streambuf_type(), _M_mode(__mode), _M_string(std::move(__s))
      { _M_stringbuf_init(__mode); }

      template<typename _SAlloc>
	basic_stringbuf(const basic_string<_CharT, _Traits, _SAlloc>& __s,
			const allocator_type& __a)
	: basic_stringbuf(__s, ios_base::in | std::ios_base::out, __a)
	{ }

      template<typename _SAlloc>
	basic_stringbuf(const basic_string<_CharT, _Traits, _SAlloc>& __s,
			ios_base::openmode __mode,
			const allocator_type& __a)
	: __streambuf_type(), _M_mode(__mode),
	  _M_string(__s.data(), __s.size(), __a)
	{ _M_stringbuf_init(__mode); }

      template<typename _SAlloc>
	explicit
	basic_stringbuf(const basic_string<_CharT, _Traits, _SAlloc>& __s,
			ios_base::openmode __mode = ios_base::in
						    | ios_base::out)
	: basic_stringbuf(__s, __mode, allocator_type{})
	{ }

      basic_stringbuf(basic_stringbuf&& __rhs, const allocator_type& __a)
      : basic_stringbuf(std::move(__rhs), __a, __xfer_bufptrs(__rhs, this))
      { __rhs._M_sync(const_cast<char_type*>(__rhs._M_string.data()), 0, 0); }

      allocator_type get_allocator() const noexcept
      { return _M_string.get_allocator(); }
#endif // C++20

      // 27.8.2.2 Assign and swap:

      basic_stringbuf&
      operator=(const basic_stringbuf&) = delete;

      basic_stringbuf&
      operator=(basic_stringbuf&& __rhs)
      {
	__xfer_bufptrs __st{__rhs, this};
	const __streambuf_type& __base = __rhs;
	__streambuf_type::operator=(__base);
	this->pubimbue(__rhs.getloc());
	_M_mode = __rhs._M_mode;
	_M_string = std::move(__rhs._M_string);
	__rhs._M_sync(const_cast<char_type*>(__rhs._M_string.data()), 0, 0);
	return *this;
      }

      void
      swap(basic_stringbuf& __rhs) noexcept(_Noexcept_swap::value)
      {
	__xfer_bufptrs __l_st{*this, std::__addressof(__rhs)};
	__xfer_bufptrs __r_st{__rhs, this};
	__streambuf_type& __base = __rhs;
	__streambuf_type::swap(__base);
	__rhs.pubimbue(this->pubimbue(__rhs.getloc()));
	std::swap(_M_mode, __rhs._M_mode);
	std::swap(_M_string, __rhs._M_string); // XXX not exception safe
      }
#endif // C++11

      // Getters and setters:

      /**
       *  @brief  Copying out the string buffer.
       *  @return  A copy of one of the underlying sequences.
       *
       *  <em>If the buffer is only created in input mode, the underlying
       *  character sequence is equal to the input sequence; otherwise, it
       *  is equal to the output sequence.</em> [27.7.1.2]/1
      */
      __string_type
      str() const _GLIBCXX_LVAL_REF_QUAL
      {
	__string_type __ret(_M_string.get_allocator());
	if (char_type* __hi = _M_high_mark())
	  __ret.assign(this->pbase(), __hi);
	else
	  __ret = _M_string;
	return __ret;
      }

#if __cplusplus > 201703L
#if _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	basic_string<_CharT, _Traits, _SAlloc>
	str(const _SAlloc& __sa) const
	{
	  auto __sv = view();
	  return { __sv.data(), __sv.size(), __sa };
	}
#endif

      __string_type
      str() &&
      {
	if (char_type* __hi = _M_high_mark())
	  {
	    // Set length to end of character sequence and add null terminator.
	    _M_string._M_set_length(_M_high_mark() - this->pbase());
	  }
	auto __str = std::move(_M_string);
	_M_string.clear();
	_M_sync(_M_string.data(), 0, 0);
	return __str;
      }
#endif // cxx11 ABI

      _GLIBCXX_SSTREAM_ALWAYS_INLINE
      basic_string_view<char_type, traits_type>
      view() const noexcept
      {
	if (char_type* __hi = _M_high_mark())
	  return { this->pbase(), __hi };
	else
	  return _M_string;
      }
#endif // C++20

      /**
       *  @brief  Setting a new buffer.
       *  @param  __s  The string to use as a new sequence.
       *
       *  Deallocates any previous stored sequence, then copies @a s to
       *  use as a new one.
      */
      void
      str(const __string_type& __s)
      {
	// Cannot use _M_string = __s, since v3 strings are COW
	// (not always true now but assign() always works).
	_M_string.assign(__s.data(), __s.size());
	_M_stringbuf_init(_M_mode);
      }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	requires (!is_same_v<_SAlloc, _Alloc>)
	void
	str(const basic_string<_CharT, _Traits, _SAlloc>& __s)
	{
	  _M_string.assign(__s.data(), __s.size());
	  _M_stringbuf_init(_M_mode);
	}
#endif

      void
      str(__string_type&& __s)
      {
	_M_string = std::move(__s);
	_M_stringbuf_init(_M_mode);
      }
#endif

    protected:
      // Common initialization code goes here.
      void
      _M_stringbuf_init(ios_base::openmode __mode)
      {
	_M_mode = __mode;
	__size_type __len = 0;
	if (_M_mode & (ios_base::ate | ios_base::app))
	  __len = _M_string.size();
	_M_sync(const_cast<char_type*>(_M_string.data()), 0, __len);
      }

      virtual streamsize
      showmanyc()
      {
	streamsize __ret = -1;
	if (_M_mode & ios_base::in)
	  {
	    _M_update_egptr();
	    __ret = this->egptr() - this->gptr();
	  }
	return __ret;
      }

      virtual int_type
      underflow();

      virtual int_type
      pbackfail(int_type __c = traits_type::eof());

      virtual int_type
      overflow(int_type __c = traits_type::eof());

      /**
       *  @brief  Manipulates the buffer.
       *  @param  __s  Pointer to a buffer area.
       *  @param  __n  Size of @a __s.
       *  @return  @c this
       *
       *  If no buffer has already been created, and both @a __s and @a __n are
       *  non-zero, then @c __s is used as a buffer; see
       *  https://gcc.gnu.org/onlinedocs/libstdc++/manual/streambufs.html#io.streambuf.buffering
       *  for more.
      */
      virtual __streambuf_type*
      setbuf(char_type* __s, streamsize __n)
      {
	if (__s && __n >= 0)
	  {
	    // This is implementation-defined behavior, and assumes
	    // that an external char_type array of length __n exists
	    // and has been pre-allocated. If this is not the case,
	    // things will quickly blow up.

	    // Step 1: Destroy the current internal array.
	    _M_string.clear();

	    // Step 2: Use the external array.
	    _M_sync(__s, __n, 0);
	  }
	return this;
      }

      virtual pos_type
      seekoff(off_type __off, ios_base::seekdir __way,
	      ios_base::openmode __mode = ios_base::in | ios_base::out);

      virtual pos_type
      seekpos(pos_type __sp,
	      ios_base::openmode __mode = ios_base::in | ios_base::out);

      // Internal function for correctly updating the internal buffer
      // for a particular _M_string, due to initialization or re-sizing
      // of an existing _M_string.
      void
      _M_sync(char_type* __base, __size_type __i, __size_type __o);

      // Internal function for correctly updating egptr() to the actual
      // string end.
      void
      _M_update_egptr()
      {
	if (char_type* __pptr = this->pptr())
	  {
	    char_type* __egptr = this->egptr();
	    if (!__egptr || __pptr > __egptr)
	      {
		if (_M_mode & ios_base::in)
		  this->setg(this->eback(), this->gptr(), __pptr);
		else
		  this->setg(__pptr, __pptr, __pptr);
	      }
	  }
      }

      // Works around the issue with pbump, part of the protected
      // interface of basic_streambuf, taking just an int.
      void
      _M_pbump(char_type* __pbeg, char_type* __pend, off_type __off);

    private:
      // Return a pointer to the end of the underlying character sequence.
      // This might not be the same character as _M_string.end() because
      // basic_stringbuf::overflow might have written to unused capacity
      // in _M_string without updating its length.
      __attribute__((__always_inline__))
      char_type*
      _M_high_mark() const _GLIBCXX_NOEXCEPT
      {
	if (char_type* __pptr = this->pptr())
	  {
	    char_type* __egptr = this->egptr();
	    if (!__egptr || __pptr > __egptr)
	      return __pptr;  // Underlying sequence is [pbase, pptr).
	    else
	      return __egptr; // Underlying sequence is [pbase, egptr).
	  }
	return 0; // Underlying character sequence is just _M_string.
      }

#if __cplusplus >= 201103L
#if _GLIBCXX_USE_CXX11_ABI
      // This type captures the state of the gptr / pptr pointers as offsets
      // so they can be restored in another object after moving the string.
      struct __xfer_bufptrs
      {
	__xfer_bufptrs(const basic_stringbuf& __from, basic_stringbuf* __to)
	: _M_to{__to}, _M_goff{-1, -1, -1}, _M_poff{-1, -1, -1}
	{
	  const _CharT* const __str = __from._M_string.data();
	  const _CharT* __end = nullptr;
	  if (__from.eback())
	    {
	      _M_goff[0] = __from.eback() - __str;
	      _M_goff[1] = __from.gptr() - __str;
	      _M_goff[2] = __from.egptr() - __str;
	      __end = __from.egptr();
	    }
	  if (__from.pbase())
	    {
	      _M_poff[0] = __from.pbase() - __str;
	      _M_poff[1] = __from.pptr() - __from.pbase();
	      _M_poff[2] = __from.epptr() - __str;
	      if (!__end || __from.pptr() > __end)
		__end = __from.pptr();
	    }

	  // Set _M_string length to the greater of the get and put areas.
	  if (__end)
	    {
	      // The const_cast avoids changing this constructor's signature,
	      // because it is exported from the dynamic library.
	      auto& __mut_from = const_cast<basic_stringbuf&>(__from);
	      __mut_from._M_string._M_length(__end - __str);
	    }
	}

	~__xfer_bufptrs()
	{
	  char_type* __str = const_cast<char_type*>(_M_to->_M_string.data());
	  if (_M_goff[0] != -1)
	    _M_to->setg(__str+_M_goff[0], __str+_M_goff[1], __str+_M_goff[2]);
	  if (_M_poff[0] != -1)
	    _M_to->_M_pbump(__str+_M_poff[0], __str+_M_poff[2], _M_poff[1]);
	}

	basic_stringbuf* _M_to;
	off_type _M_goff[3];
	off_type _M_poff[3];
      };
#else
      // This type does nothing when using Copy-On-Write strings.
      struct __xfer_bufptrs
      {
	__xfer_bufptrs(const basic_stringbuf&, basic_stringbuf*) { }
      };
#endif

      // The move constructor initializes an __xfer_bufptrs temporary then
      // delegates to this constructor to performs moves during its lifetime.
      basic_stringbuf(basic_stringbuf&& __rhs, __xfer_bufptrs&&)
      : __streambuf_type(static_cast<const __streambuf_type&>(__rhs)),
      _M_mode(__rhs._M_mode), _M_string(std::move(__rhs._M_string))
      { }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
      // The move constructor initializes an __xfer_bufptrs temporary then
      // delegates to this constructor to performs moves during its lifetime.
      basic_stringbuf(basic_stringbuf&& __rhs, const allocator_type& __a,
		      __xfer_bufptrs&&)
      : __streambuf_type(static_cast<const __streambuf_type&>(__rhs)),
      _M_mode(__rhs._M_mode), _M_string(std::move(__rhs._M_string), __a)
      { }
#endif
#endif // C++11
    };


  // [27.7.2] Template class basic_istringstream
  /**
   *  @brief  Controlling input for std::string.
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *  @tparam _Alloc  Allocator type, defaults to allocator<_CharT>.
   *
   *  This class supports reading from objects of type std::basic_string,
   *  using the inherited functions from std::basic_istream.  To control
   *  the associated sequence, an instance of std::basic_stringbuf is used,
   *  which this page refers to as @c sb.
  */
  template<typename _CharT, typename _Traits, typename _Alloc>
    class basic_istringstream : public basic_istream<_CharT, _Traits>
    {
    public:
      // Types:
      typedef _CharT 					char_type;
      typedef _Traits 					traits_type;
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 251. basic_stringbuf missing allocator_type
      typedef _Alloc				       	allocator_type;
      typedef typename traits_type::int_type 		int_type;
      typedef typename traits_type::pos_type 		pos_type;
      typedef typename traits_type::off_type 		off_type;

      // Non-standard types:
      typedef basic_string<_CharT, _Traits, _Alloc> 	__string_type;
      typedef basic_stringbuf<_CharT, _Traits, _Alloc> 	__stringbuf_type;
      typedef basic_istream<char_type, traits_type>	__istream_type;

    private:
      __stringbuf_type	_M_stringbuf;

    public:
      // Constructors:

      /**
       *  @brief  Default constructor starts with an empty string buffer.
       *
       *  Initializes @c sb using @c in, and passes @c &sb to the base
       *  class initializer.  Does not allocate any buffer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      basic_istringstream()
      : __istream_type(), _M_stringbuf(ios_base::in)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  Starts with an empty string buffer.
       *  @param  __mode  Whether the buffer can read, or write, or both.
       *
       *  @c ios_base::in is automatically included in @a __mode.
       *
       *  Initializes @c sb using @c __mode|in, and passes @c &sb to the base
       *  class initializer.  Does not allocate any buffer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      explicit
      basic_istringstream(ios_base::openmode __mode)
      : __istream_type(), _M_stringbuf(__mode | ios_base::in)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  Starts with an existing string buffer.
       *  @param  __str  A string to copy as a starting buffer.
       *  @param  __mode  Whether the buffer can read, or write, or both.
       *
       *  @c ios_base::in is automatically included in @a mode.
       *
       *  Initializes @c sb using @a str and @c mode|in, and passes @c &sb
       *  to the base class initializer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      explicit
      basic_istringstream(const __string_type& __str,
			  ios_base::openmode __mode = ios_base::in)
      : __istream_type(), _M_stringbuf(__str, __mode | ios_base::in)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  The destructor does nothing.
       *
       *  The buffer is deallocated by the stringbuf object, not the
       *  formatting stream.
      */
      ~basic_istringstream()
      { }

#if __cplusplus >= 201103L
      basic_istringstream(const basic_istringstream&) = delete;

      basic_istringstream(basic_istringstream&& __rhs)
      : __istream_type(std::move(__rhs)),
      _M_stringbuf(std::move(__rhs._M_stringbuf))
      { __istream_type::set_rdbuf(&_M_stringbuf); }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
      basic_istringstream(ios_base::openmode __mode, const allocator_type& __a)
      : __istream_type(), _M_stringbuf(__mode | ios_base::in, __a)
      { this->init(std::__addressof(_M_stringbuf)); }

      explicit
      basic_istringstream(__string_type&& __str,
			  ios_base::openmode __mode = ios_base::in)
      : __istream_type(), _M_stringbuf(std::move(__str), __mode | ios_base::in)
      { this->init(std::__addressof(_M_stringbuf)); }

      template<typename _SAlloc>
	basic_istringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			    const allocator_type& __a)
	: basic_istringstream(__str, ios_base::in, __a)
	{ }

      template<typename _SAlloc>
	basic_istringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			    ios_base::openmode __mode,
			    const allocator_type& __a)
	: __istream_type(), _M_stringbuf(__str, __mode | ios_base::in, __a)
	{ this->init(std::__addressof(_M_stringbuf)); }

      template<typename _SAlloc>
	explicit
	basic_istringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			    ios_base::openmode __mode = ios_base::in)
	: basic_istringstream(__str, __mode, allocator_type())
	{ }
#endif // C++20

      // 27.8.3.2 Assign and swap:

      basic_istringstream&
      operator=(const basic_istringstream&) = delete;

      basic_istringstream&
      operator=(basic_istringstream&& __rhs)
      {
	__istream_type::operator=(std::move(__rhs));
	_M_stringbuf = std::move(__rhs._M_stringbuf);
	return *this;
      }

      void
      swap(basic_istringstream& __rhs)
      {
	__istream_type::swap(__rhs);
	_M_stringbuf.swap(__rhs._M_stringbuf);
      }
#endif // C++11

      // Members:
      /**
       *  @brief  Accessing the underlying buffer.
       *  @return  The current basic_stringbuf buffer.
       *
       *  This hides both signatures of std::basic_ios::rdbuf().
      */
      __stringbuf_type*
      rdbuf() const
      { return const_cast<__stringbuf_type*>(&_M_stringbuf); }

      /**
       *  @brief  Copying out the string buffer.
       *  @return  @c rdbuf()->str()
      */
      __string_type
      str() const _GLIBCXX_LVAL_REF_QUAL
      { return _M_stringbuf.str(); }

#if __cplusplus > 201703L
#if _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	basic_string<_CharT, _Traits, _SAlloc>
	str(const _SAlloc& __sa) const
	{ return _M_stringbuf.str(__sa); }
#endif

      __string_type
      str() &&
      { return std::move(_M_stringbuf).str(); }
#endif // cxx11 ABI

      _GLIBCXX_SSTREAM_ALWAYS_INLINE
      basic_string_view<char_type, traits_type>
      view() const noexcept
      { return _M_stringbuf.view(); }
#endif // C++20

      /**
       *  @brief  Setting a new buffer.
       *  @param  __s  The string to use as a new sequence.
       *
       *  Calls @c rdbuf()->str(s).
      */
      void
      str(const __string_type& __s)
      { _M_stringbuf.str(__s); }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	requires (!is_same_v<_SAlloc, _Alloc>)
	void
	str(const basic_string<_CharT, _Traits, _SAlloc>& __s)
	{ _M_stringbuf.str(__s); }
#endif

      void
      str(__string_type&& __s)
      { _M_stringbuf.str(std::move(__s)); }
#endif
    };


  // [27.7.3] Template class basic_ostringstream
  /**
   *  @brief  Controlling output for std::string.
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *  @tparam _Alloc  Allocator type, defaults to allocator<_CharT>.
   *
   *  This class supports writing to objects of type std::basic_string,
   *  using the inherited functions from std::basic_ostream.  To control
   *  the associated sequence, an instance of std::basic_stringbuf is used,
   *  which this page refers to as @c sb.
  */
  template <typename _CharT, typename _Traits, typename _Alloc>
    class basic_ostringstream : public basic_ostream<_CharT, _Traits>
    {
    public:
      // Types:
      typedef _CharT 					char_type;
      typedef _Traits 					traits_type;
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 251. basic_stringbuf missing allocator_type
      typedef _Alloc				       	allocator_type;
      typedef typename traits_type::int_type 		int_type;
      typedef typename traits_type::pos_type 		pos_type;
      typedef typename traits_type::off_type 		off_type;

      // Non-standard types:
      typedef basic_string<_CharT, _Traits, _Alloc> 	__string_type;
      typedef basic_stringbuf<_CharT, _Traits, _Alloc> 	__stringbuf_type;
      typedef basic_ostream<char_type, traits_type>	__ostream_type;

    private:
      __stringbuf_type	_M_stringbuf;

    public:
      // Constructors/destructor:

      /**
       *  @brief  Default constructor starts with an empty string buffer.
       *
       *  Initializes @c sb using @c mode|out, and passes @c &sb to the base
       *  class initializer.  Does not allocate any buffer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      basic_ostringstream()
      : __ostream_type(), _M_stringbuf(ios_base::out)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  Starts with an empty string buffer.
       *  @param  __mode  Whether the buffer can read, or write, or both.
       *
       *  @c ios_base::out is automatically included in @a mode.
       *
       *  Initializes @c sb using @c mode|out, and passes @c &sb to the base
       *  class initializer.  Does not allocate any buffer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      explicit
      basic_ostringstream(ios_base::openmode __mode)
      : __ostream_type(), _M_stringbuf(__mode | ios_base::out)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  Starts with an existing string buffer.
       *  @param  __str  A string to copy as a starting buffer.
       *  @param  __mode  Whether the buffer can read, or write, or both.
       *
       *  @c ios_base::out is automatically included in @a mode.
       *
       *  Initializes @c sb using @a str and @c mode|out, and passes @c &sb
       *  to the base class initializer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      explicit
      basic_ostringstream(const __string_type& __str,
			  ios_base::openmode __mode = ios_base::out)
      : __ostream_type(), _M_stringbuf(__str, __mode | ios_base::out)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  The destructor does nothing.
       *
       *  The buffer is deallocated by the stringbuf object, not the
       *  formatting stream.
      */
      ~basic_ostringstream()
      { }

#if __cplusplus >= 201103L
      basic_ostringstream(const basic_ostringstream&) = delete;

      basic_ostringstream(basic_ostringstream&& __rhs)
      : __ostream_type(std::move(__rhs)),
      _M_stringbuf(std::move(__rhs._M_stringbuf))
      { __ostream_type::set_rdbuf(&_M_stringbuf); }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
      basic_ostringstream(ios_base::openmode __mode, const allocator_type& __a)
      : __ostream_type(), _M_stringbuf(__mode | ios_base::out, __a)
      { this->init(std::__addressof(_M_stringbuf)); }

      explicit
      basic_ostringstream(__string_type&& __str,
			  ios_base::openmode __mode = ios_base::out)
      : __ostream_type(), _M_stringbuf(std::move(__str), __mode | ios_base::out)
      { this->init(std::__addressof(_M_stringbuf)); }

      template<typename _SAlloc>
	basic_ostringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			    const allocator_type& __a)
	: basic_ostringstream(__str, ios_base::out, __a)
	{ }

      template<typename _SAlloc>
	basic_ostringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			    ios_base::openmode __mode,
			    const allocator_type& __a)
	: __ostream_type(), _M_stringbuf(__str, __mode | ios_base::out, __a)
	{ this->init(std::__addressof(_M_stringbuf)); }

      template<typename _SAlloc>
	explicit
	basic_ostringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			    ios_base::openmode __mode = ios_base::out)
	: basic_ostringstream(__str, __mode, allocator_type())
	{ }
#endif // C++20

      // 27.8.3.2 Assign and swap:

      basic_ostringstream&
      operator=(const basic_ostringstream&) = delete;

      basic_ostringstream&
      operator=(basic_ostringstream&& __rhs)
      {
	__ostream_type::operator=(std::move(__rhs));
	_M_stringbuf = std::move(__rhs._M_stringbuf);
	return *this;
      }

      void
      swap(basic_ostringstream& __rhs)
      {
	__ostream_type::swap(__rhs);
	_M_stringbuf.swap(__rhs._M_stringbuf);
      }
#endif // C++11

      // Members:
      /**
       *  @brief  Accessing the underlying buffer.
       *  @return  The current basic_stringbuf buffer.
       *
       *  This hides both signatures of std::basic_ios::rdbuf().
      */
      __stringbuf_type*
      rdbuf() const
      { return const_cast<__stringbuf_type*>(&_M_stringbuf); }

      /**
       *  @brief  Copying out the string buffer.
       *  @return  @c rdbuf()->str()
      */
      __string_type
      str() const _GLIBCXX_LVAL_REF_QUAL
      { return _M_stringbuf.str(); }

#if __cplusplus > 201703L
#if _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	basic_string<_CharT, _Traits, _SAlloc>
	str(const _SAlloc& __sa) const
	{ return _M_stringbuf.str(__sa); }
#endif

      __string_type
      str() &&
      { return std::move(_M_stringbuf).str(); }
#endif // cxx11 ABI

      _GLIBCXX_SSTREAM_ALWAYS_INLINE
      basic_string_view<char_type, traits_type>
      view() const noexcept
      { return _M_stringbuf.view(); }
#endif // C++20

      /**
       *  @brief  Setting a new buffer.
       *  @param  __s  The string to use as a new sequence.
       *
       *  Calls @c rdbuf()->str(s).
      */
      void
      str(const __string_type& __s)
      { _M_stringbuf.str(__s); }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	requires (!is_same_v<_SAlloc, _Alloc>)
	void
	str(const basic_string<_CharT, _Traits, _SAlloc>& __s)
	{ _M_stringbuf.str(__s); }
#endif

      void
      str(__string_type&& __s)
      { _M_stringbuf.str(std::move(__s)); }
#endif
    };


  // [27.7.4] Template class basic_stringstream
  /**
   *  @brief  Controlling input and output for std::string.
   *  @ingroup io
   *
   *  @tparam _CharT  Type of character stream.
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *  @tparam _Alloc  Allocator type, defaults to allocator<_CharT>.
   *
   *  This class supports reading from and writing to objects of type
   *  std::basic_string, using the inherited functions from
   *  std::basic_iostream.  To control the associated sequence, an instance
   *  of std::basic_stringbuf is used, which this page refers to as @c sb.
  */
  template <typename _CharT, typename _Traits, typename _Alloc>
    class basic_stringstream : public basic_iostream<_CharT, _Traits>
    {
    public:
      // Types:
      typedef _CharT 					char_type;
      typedef _Traits 					traits_type;
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 251. basic_stringbuf missing allocator_type
      typedef _Alloc				       	allocator_type;
      typedef typename traits_type::int_type 		int_type;
      typedef typename traits_type::pos_type 		pos_type;
      typedef typename traits_type::off_type 		off_type;

      // Non-standard Types:
      typedef basic_string<_CharT, _Traits, _Alloc> 	__string_type;
      typedef basic_stringbuf<_CharT, _Traits, _Alloc> 	__stringbuf_type;
      typedef basic_iostream<char_type, traits_type>	__iostream_type;

    private:
      __stringbuf_type	_M_stringbuf;

    public:
      // Constructors/destructors

      /**
       *  @brief  Default constructor starts with an empty string buffer.
       *
       *  Initializes @c sb using the mode @c in|out, and passes @c &sb
       *  to the base class initializer.  Does not allocate any buffer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      basic_stringstream()
      : __iostream_type(), _M_stringbuf(ios_base::out | ios_base::in)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  Starts with an empty string buffer.
       *  @param  __m  Whether the buffer can read, or write, or both.
       *
       *  Initializes @c sb using the mode from @c __m, and passes @c &sb
       *  to the base class initializer.  Does not allocate any buffer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      explicit
      basic_stringstream(ios_base::openmode __m)
      : __iostream_type(), _M_stringbuf(__m)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  Starts with an existing string buffer.
       *  @param  __str  A string to copy as a starting buffer.
       *  @param  __m  Whether the buffer can read, or write, or both.
       *
       *  Initializes @c sb using @a __str and @c __m, and passes @c &sb
       *  to the base class initializer.
       *
       *  That's a lie.  We initialize the base class with NULL, because the
       *  string class does its own memory management.
      */
      explicit
      basic_stringstream(const __string_type& __str,
			 ios_base::openmode __m = ios_base::out | ios_base::in)
      : __iostream_type(), _M_stringbuf(__str, __m)
      { this->init(&_M_stringbuf); }

      /**
       *  @brief  The destructor does nothing.
       *
       *  The buffer is deallocated by the stringbuf object, not the
       *  formatting stream.
      */
      ~basic_stringstream()
      { }

#if __cplusplus >= 201103L
      basic_stringstream(const basic_stringstream&) = delete;

      basic_stringstream(basic_stringstream&& __rhs)
      : __iostream_type(std::move(__rhs)),
      _M_stringbuf(std::move(__rhs._M_stringbuf))
      { __iostream_type::set_rdbuf(&_M_stringbuf); }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
      basic_stringstream(ios_base::openmode __mode, const allocator_type& __a)
      : __iostream_type(), _M_stringbuf(__mode, __a)
      { this->init(&_M_stringbuf); }

      explicit
      basic_stringstream(__string_type&& __str,
			 ios_base::openmode __mode = ios_base::in
						     | ios_base::out)
      : __iostream_type(), _M_stringbuf(std::move(__str), __mode)
      { this->init(std::__addressof(_M_stringbuf)); }

      template<typename _SAlloc>
	basic_stringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			   const allocator_type& __a)
	: basic_stringstream(__str, ios_base::in | ios_base::out, __a)
	{ }

      template<typename _SAlloc>
	basic_stringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			   ios_base::openmode __mode,
			   const allocator_type& __a)
	: __iostream_type(), _M_stringbuf(__str, __mode, __a)
	{ this->init(std::__addressof(_M_stringbuf)); }

      template<typename _SAlloc>
	explicit
	basic_stringstream(const basic_string<_CharT, _Traits, _SAlloc>& __str,
			   ios_base::openmode __mode = ios_base::in
						       | ios_base::out)
	: basic_stringstream(__str, __mode, allocator_type())
	{ }
#endif // C++20

      // 27.8.3.2 Assign and swap:

      basic_stringstream&
      operator=(const basic_stringstream&) = delete;

      basic_stringstream&
      operator=(basic_stringstream&& __rhs)
      {
	__iostream_type::operator=(std::move(__rhs));
	_M_stringbuf = std::move(__rhs._M_stringbuf);
	return *this;
      }

      void
      swap(basic_stringstream& __rhs)
      {
	__iostream_type::swap(__rhs);
	_M_stringbuf.swap(__rhs._M_stringbuf);
      }
#endif // C++11

      // Members:
      /**
       *  @brief  Accessing the underlying buffer.
       *  @return  The current basic_stringbuf buffer.
       *
       *  This hides both signatures of std::basic_ios::rdbuf().
      */
      __stringbuf_type*
      rdbuf() const
      { return const_cast<__stringbuf_type*>(&_M_stringbuf); }

      /**
       *  @brief  Copying out the string buffer.
       *  @return  @c rdbuf()->str()
      */
      __string_type
      str() const _GLIBCXX_LVAL_REF_QUAL
      { return _M_stringbuf.str(); }

#if __cplusplus > 201703L
#if _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	basic_string<_CharT, _Traits, _SAlloc>
	str(const _SAlloc& __sa) const
	{ return _M_stringbuf.str(__sa); }
#endif

      __string_type
      str() &&
      { return std::move(_M_stringbuf).str(); }
#endif // cxx11 ABI

      _GLIBCXX_SSTREAM_ALWAYS_INLINE
      basic_string_view<char_type, traits_type>
      view() const noexcept
      { return _M_stringbuf.view(); }
#endif // C++20

      /**
       *  @brief  Setting a new buffer.
       *  @param  __s  The string to use as a new sequence.
       *
       *  Calls @c rdbuf()->str(s).
      */
      void
      str(const __string_type& __s)
      { _M_stringbuf.str(__s); }

#if __cplusplus > 201703L && _GLIBCXX_USE_CXX11_ABI
#if __cpp_concepts
      template<__allocator_like _SAlloc>
	requires (!is_same_v<_SAlloc, _Alloc>)
	void
	str(const basic_string<_CharT, _Traits, _SAlloc>& __s)
	{ _M_stringbuf.str(__s); }
#endif

      void
      str(__string_type&& __s)
      { _M_stringbuf.str(std::move(__s)); }
#endif
    };

#if __cplusplus >= 201103L
  /// Swap specialization for stringbufs.
  template <class _CharT, class _Traits, class _Allocator>
    inline void
    swap(basic_stringbuf<_CharT, _Traits, _Allocator>& __x,
	 basic_stringbuf<_CharT, _Traits, _Allocator>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }

  /// Swap specialization for istringstreams.
  template <class _CharT, class _Traits, class _Allocator>
    inline void
    swap(basic_istringstream<_CharT, _Traits, _Allocator>& __x,
	 basic_istringstream<_CharT, _Traits, _Allocator>& __y)
    { __x.swap(__y); }

  /// Swap specialization for ostringstreams.
  template <class _CharT, class _Traits, class _Allocator>
    inline void
    swap(basic_ostringstream<_CharT, _Traits, _Allocator>& __x,
	 basic_ostringstream<_CharT, _Traits, _Allocator>& __y)
    { __x.swap(__y); }

  /// Swap specialization for stringstreams.
  template <class _CharT, class _Traits, class _Allocator>
    inline void
    swap(basic_stringstream<_CharT, _Traits, _Allocator>& __x,
	 basic_stringstream<_CharT, _Traits, _Allocator>& __y)
    { __x.swap(__y); }
#endif // C++11

_GLIBCXX_END_NAMESPACE_CXX11
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#undef _GLIBCXX_SSTREAM_ALWAYS_INLINE
#undef _GLIBCXX_LVAL_REF_QUAL

#include <bits/sstream.tcc>

#endif /* _GLIBCXX_SSTREAM */
