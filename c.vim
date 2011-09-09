" Vim syntax file
" Language:	C
" Maintainer:	Bram Moolenaar <Bram@vim.org>
" Last Change:	2009 Nov 17

" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

" OpenGL Types
syn keyword cType    GLfloat GLdouble GLbyte GLubyte GLint GLuint GLsizei

" 'cConditional' is a hack to get the coloring I want.
syn keyword cConditional __attribute__

" A bunch of useful C keywords
syn keyword	cStatement	goto break return continue asm
syn keyword	cLabel		case default
syn keyword	cConditional	if else switch
syn keyword	cRepeat		while for do

syn keyword	cTodo		contained TODO FIXME XXX

" It's easy to accidentally add a space after a backslash that was intended
" for line continuation.  Some compilers allow it, which makes it
" unpredicatable and should be avoided.
syn match	cBadContinuation contained "\\\s\+$"

" cCommentGroup allows adding matches for special things in comments
syn cluster	cCommentGroup	contains=cTodo,cBadContinuation

" String and Character constants
" Highlight special characters (those which have a backslash) differently
syn match	cSpecial	display contained "\\\(x\x\+\|\o\{1,3}\|.\|$\)"
if !exists("c_no_utf")
  syn match	cSpecial	display contained "\\\(u\x\{4}\|U\x\{8}\)"
endif
if exists("c_no_cformat")
  syn region	cString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,@Spell
  " cCppString: same as cString, but ends at end of line
  syn region	cCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,@Spell
else
  if !exists("c_no_c99") " ISO C99
    syn match	cFormat		display "%\(\d\+\$\)\=[-+' #0*]*\(\d*\|\*\|\*\d\+\$\)\(\.\(\d*\|\*\|\*\d\+\$\)\)\=\([hlLjzt]\|ll\|hh\)\=\([aAbdiuoxXDOUfFeEgGcCsSpn]\|\[\^\=.[^]]*\]\)" contained
  else
    syn match	cFormat		display "%\(\d\+\$\)\=[-+' #0*]*\(\d*\|\*\|\*\d\+\$\)\(\.\(\d*\|\*\|\*\d\+\$\)\)\=\([hlL]\|ll\)\=\([bdiuoxXDOUfeEgGcCsSpn]\|\[\^\=.[^]]*\]\)" contained
  endif
  syn match	cFormat		display "%%" contained
  syn region	cString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,cFormat,@Spell
  " cCppString: same as cString, but ends at end of line
  syn region	cCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,cFormat,@Spell
endif

syn match	cCharacter	"L\='[^\\]'"
syn match	cCharacter	"L'[^']*'" contains=cSpecial
if exists("c_gnu")
  syn match	cSpecialError	"L\='\\[^'\"?\\abefnrtv]'"
  syn match	cSpecialCharacter "L\='\\['\"?\\abefnrtv]'"
else
  syn match	cSpecialError	"L\='\\[^'\"?\\abfnrtv]'"
  syn match	cSpecialCharacter "L\='\\['\"?\\abfnrtv]'"
endif
syn match	cSpecialCharacter display "L\='\\\o\{1,3}'"
syn match	cSpecialCharacter display "'\\x\x\{1,2}'"
syn match	cSpecialCharacter display "L'\\x\x\+'"

"when wanted, highlight trailing white space
if exists("c_space_errors")
  if !exists("c_no_trail_space_error")
    syn match	cSpaceError	display excludenl "\s\+$"
  endif
  if !exists("c_no_tab_space_error")
    syn match	cSpaceError	display " \+\t"me=e-1
  endif
endif

" This should be before cErrInParen to avoid problems with #define ({ xxx })
if exists("c_curly_error")
  syntax match cCurlyError "}"
  syntax region	cBlock		start="{" end="}" contains=ALLBUT,cCurlyError,@cParenGroup,cErrInParen,cCppParen,cErrInBracket,cCppBracket,cCppString,@Spell fold
else
  syntax region	cBlock		start="{" end="}" transparent fold
endif

"catch errors caused by wrong parenthesis and brackets
" also accept <% for {, %> for }, <: for [ and :> for ] (C99)
" But avoid matching <::.
syn cluster	cParenGroup	contains=cParenError,cIncluded,cSpecial,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cUserCont,cUserLabel,cBitField,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom
if exists("c_no_curly_error")
  syn region	cParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cCppString,@Spell
  " cCppParen: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cParen,cString,@Spell
  syn match	cParenError	display ")"
  syn match	cErrInParen	display contained "^[{}]\|^<%\|^%>"
elseif exists("c_no_bracket_error")
  syn region	cParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cCppString,@Spell
  " cCppParen: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cParen,cString,@Spell
  syn match	cParenError	display ")"
  syn match	cErrInParen	display contained "[{}]\|<%\|%>"
else
  syn region	cParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cErrInBracket,cCppBracket,cCppString,@Spell
  " cCppParen: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cErrInBracket,cParen,cBracket,cString,@Spell
  syn match	cParenError	display "[\])]"
  syn match	cErrInParen	display contained "[\]{}]\|<%\|%>"
  syn region	cBracket	transparent start='\[\|<::\@!' end=']\|:>' contains=ALLBUT,@cParenGroup,cErrInParen,cCppParen,cCppBracket,cCppString,@Spell
  " cCppBracket: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppBracket	transparent start='\[\|<::\@!' skip='\\$' excludenl end=']\|:>' end='$' contained contains=ALLBUT,@cParenGroup,cErrInParen,cParen,cBracket,cString,@Spell
  syn match	cErrInBracket	display contained "[);{}]\|<%\|%>"
endif

"integer number, or floating point number without a dot and with "f".
syn case ignore
syn match	cNumbers	display transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctalError,cOctal
" Same, but without octal error (for comments)
syn match	cNumbersCom	display contained transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctal
syn match	cNumber		display contained "\d\+\(u\=l\{0,2}\|ll\=u\)\>"
"hex number
syn match	cNumber		display contained "0x\x\+\(u\=l\{0,2}\|ll\=u\)\>"
" Flag the first zero of an octal number as something special
syn match	cOctal		display contained "0\o\+\(u\=l\{0,2}\|ll\=u\)\>" contains=cOctalZero
syn match	cOctalZero	display contained "\<0"
syn match	cFloat		display contained "\d\+f"
"floating point number, with dot, optional exponent
syn match	cFloat		display contained "\d\+\.\d*\(e[-+]\=\d\+\)\=[fl]\="
"floating point number, starting with a dot, optional exponent
syn match	cFloat		display contained "\.\d\+\(e[-+]\=\d\+\)\=[fl]\=\>"
"floating point number, without dot, with exponent
syn match	cFloat		display contained "\d\+e[-+]\=\d\+[fl]\=\>"
if !exists("c_no_c99")
  "hexadecimal floating point number, optional leading digits, with dot, with exponent
  syn match	cFloat		display contained "0x\x*\.\x\+p[-+]\=\d\+[fl]\=\>"
  "hexadecimal floating point number, with leading digits, optional dot, with exponent
  syn match	cFloat		display contained "0x\x\+\.\=p[-+]\=\d\+[fl]\=\>"
endif

" flag an octal number with wrong digits
syn match	cOctalError	display contained "0\o*[89]\d*"
syn case match

if exists("c_comment_strings")
  " A comment can contain cString, cCharacter and cNumber.
  " But a "*/" inside a cString in a cComment DOES end the comment!  So we
  " need to use a special type of cString: cCommentString, which also ends on
  " "*/", and sees a "*" at the start of the line as comment again.
  " Unfortunately this doesn't very well work for // type of comments :-(
  syntax match	cCommentSkip	contained "^\s*\*\($\|\s\+\)"
  syntax region cCommentString	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end=+\*/+me=s-1 contains=cSpecial,cCommentSkip
  syntax region cComment2String	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end="$" contains=cSpecial
  syntax region  cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cComment2String,cCharacter,cNumbersCom,cSpaceError,@Spell
  if exists("c_no_comment_fold")
    " Use "extend" here to have preprocessor lines not terminate halfway a
    " comment.
    syntax region cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell extend
  else
    syntax region cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell fold extend
  endif
else
  syn region	cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cSpaceError,@Spell
  if exists("c_no_comment_fold")
    syn region	cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cSpaceError,@Spell extend
  else
    syn region	cComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cSpaceError,@Spell fold extend
  endif
endif
" keep a // comment separately, it terminates a preproc. conditional
syntax match	cCommentError	display "\*/"
syntax match	cCommentStartError display "/\*"me=e-1 contained

syn keyword	cOperator	sizeof
if exists("c_gnu")
  syn keyword	cStatement	__asm__
  syn keyword	cOperator	typeof __real__ __imag__
endif

syn keyword	cType		int long short char void
syn keyword	cType		signed unsigned float double
if !exists("c_no_ansi") || exists("c_ansi_typedefs")
  syn keyword   cType		size_t ssize_t off_t wchar_t ptrdiff_t sig_atomic_t fpos_t
  syn keyword   cType		clock_t time_t va_list jmp_buf FILE DIR div_t ldiv_t
  syn keyword   cType		mbstate_t wctrans_t wint_t wctype_t
endif
if !exists("c_no_c99") " ISO C99
  syn keyword	cType		bool complex
  syn keyword	cType		int8_t int16_t int32_t int64_t
  syn keyword	cType		uint8_t uint16_t uint32_t uint64_t
  syn keyword	cType		int_least8_t int_least16_t int_least32_t int_least64_t
  syn keyword	cType		uint_least8_t uint_least16_t uint_least32_t uint_least64_t
  syn keyword	cType		int_fast8_t int_fast16_t int_fast32_t int_fast64_t
  syn keyword	cType		uint_fast8_t uint_fast16_t uint_fast32_t uint_fast64_t
  syn keyword	cType		intptr_t uintptr_t
  syn keyword	cType		intmax_t uintmax_t
endif
if exists("c_gnu")
  syn keyword	cType		__label__ __complex__ __volatile__
endif

syn keyword	cStructure	struct union enum typedef
syn keyword	cStorageClass	static register auto volatile extern const
if exists("c_gnu")
  syn keyword	cStorageClass	inline __attribute__
endif
if !exists("c_no_c99")
  syn keyword	cStorageClass	inline restrict
endif

if !exists("c_no_ansi") || exists("c_ansi_constants") || exists("c_gnu")
  if exists("c_gnu")
    syn keyword cConstant __GNUC__ __FUNCTION__ __PRETTY_FUNCTION__ __func__
  endif
  syn keyword cConstant __LINE__ __FILE__ __DATE__ __TIME__ __STDC__
  syn keyword cConstant __STDC_VERSION__
  syn keyword cConstant CHAR_BIT MB_LEN_MAX MB_CUR_MAX
  syn keyword cConstant UCHAR_MAX UINT_MAX ULONG_MAX USHRT_MAX
  syn keyword cConstant CHAR_MIN INT_MIN LONG_MIN SHRT_MIN
  syn keyword cConstant CHAR_MAX INT_MAX LONG_MAX SHRT_MAX
  syn keyword cConstant SCHAR_MIN SINT_MIN SLONG_MIN SSHRT_MIN
  syn keyword cConstant SCHAR_MAX SINT_MAX SLONG_MAX SSHRT_MAX
  if !exists("c_no_c99")
    syn keyword cConstant __func__
    syn keyword cConstant LLONG_MIN LLONG_MAX ULLONG_MAX
    syn keyword cConstant INT8_MIN INT16_MIN INT32_MIN INT64_MIN
    syn keyword cConstant INT8_MAX INT16_MAX INT32_MAX INT64_MAX
    syn keyword cConstant UINT8_MAX UINT16_MAX UINT32_MAX UINT64_MAX
    syn keyword cConstant INT_LEAST8_MIN INT_LEAST16_MIN INT_LEAST32_MIN INT_LEAST64_MIN
    syn keyword cConstant INT_LEAST8_MAX INT_LEAST16_MAX INT_LEAST32_MAX INT_LEAST64_MAX
    syn keyword cConstant UINT_LEAST8_MAX UINT_LEAST16_MAX UINT_LEAST32_MAX UINT_LEAST64_MAX
    syn keyword cConstant INT_FAST8_MIN INT_FAST16_MIN INT_FAST32_MIN INT_FAST64_MIN
    syn keyword cConstant INT_FAST8_MAX INT_FAST16_MAX INT_FAST32_MAX INT_FAST64_MAX
    syn keyword cConstant UINT_FAST8_MAX UINT_FAST16_MAX UINT_FAST32_MAX UINT_FAST64_MAX
    syn keyword cConstant INTPTR_MIN INTPTR_MAX UINTPTR_MAX
    syn keyword cConstant INTMAX_MIN INTMAX_MAX UINTMAX_MAX
    syn keyword cConstant PTRDIFF_MIN PTRDIFF_MAX SIG_ATOMIC_MIN SIG_ATOMIC_MAX
    syn keyword cConstant SIZE_MAX WCHAR_MIN WCHAR_MAX WINT_MIN WINT_MAX
  endif
  syn keyword cConstant FLT_RADIX FLT_ROUNDS
  syn keyword cConstant FLT_DIG FLT_MANT_DIG FLT_EPSILON
  syn keyword cConstant DBL_DIG DBL_MANT_DIG DBL_EPSILON
  syn keyword cConstant LDBL_DIG LDBL_MANT_DIG LDBL_EPSILON
  syn keyword cConstant FLT_MIN FLT_MAX FLT_MIN_EXP FLT_MAX_EXP
  syn keyword cConstant FLT_MIN_10_EXP FLT_MAX_10_EXP
  syn keyword cConstant DBL_MIN DBL_MAX DBL_MIN_EXP DBL_MAX_EXP
  syn keyword cConstant DBL_MIN_10_EXP DBL_MAX_10_EXP
  syn keyword cConstant LDBL_MIN LDBL_MAX LDBL_MIN_EXP LDBL_MAX_EXP
  syn keyword cConstant LDBL_MIN_10_EXP LDBL_MAX_10_EXP
  syn keyword cConstant HUGE_VAL CLOCKS_PER_SEC NULL
  syn keyword cConstant LC_ALL LC_COLLATE LC_CTYPE LC_MONETARY
  syn keyword cConstant LC_NUMERIC LC_TIME
  syn keyword cConstant SIG_DFL SIG_ERR SIG_IGN
  syn keyword cConstant SIGABRT SIGFPE SIGILL SIGHUP SIGINT SIGSEGV SIGTERM
  " Add POSIX signals as well...
  syn keyword cConstant SIGABRT SIGALRM SIGCHLD SIGCONT SIGFPE SIGHUP
  syn keyword cConstant SIGILL SIGINT SIGKILL SIGPIPE SIGQUIT SIGSEGV
  syn keyword cConstant SIGSTOP SIGTERM SIGTRAP SIGTSTP SIGTTIN SIGTTOU
  syn keyword cConstant SIGUSR1 SIGUSR2
  syn keyword cConstant _IOFBF _IOLBF _IONBF BUFSIZ EOF WEOF
  syn keyword cConstant FOPEN_MAX FILENAME_MAX L_tmpnam
  syn keyword cConstant SEEK_CUR SEEK_END SEEK_SET
  syn keyword cConstant TMP_MAX stderr stdin stdout
  syn keyword cConstant EXIT_FAILURE EXIT_SUCCESS RAND_MAX
  " Add POSIX errors as well
  syn keyword cConstant E2BIG EACCES EAGAIN EBADF EBADMSG EBUSY
  syn keyword cConstant ECANCELED ECHILD EDEADLK EDOM EEXIST EFAULT
  syn keyword cConstant EFBIG EILSEQ EINPROGRESS EINTR EINVAL EIO EISDIR
  syn keyword cConstant EMFILE EMLINK EMSGSIZE ENAMETOOLONG ENFILE ENODEV
  syn keyword cConstant ENOENT ENOEXEC ENOLCK ENOMEM ENOSPC ENOSYS
  syn keyword cConstant ENOTDIR ENOTEMPTY ENOTSUP ENOTTY ENXIO EPERM
  syn keyword cConstant EPIPE ERANGE EROFS ESPIPE ESRCH ETIMEDOUT EXDEV
  " math.h
  syn keyword cConstant M_E M_LOG2E M_LOG10E M_LN2 M_LN10 M_PI M_PI_2 M_PI_4
  syn keyword cConstant M_1_PI M_2_PI M_2_SQRTPI M_SQRT2 M_SQRT1_2
endif
if !exists("c_no_c99") " ISO C99
  syn keyword cConstant true false
endif

syn keyword cConstant GL_SHORT GL_INT GL_FLOAT GL_DOUBLE GL_BYTE
syn keyword cConstant GL_UNSIGNED_BYTE GL_UNSIGNED_INT GL_UNSIGNED_SHORT
syn keyword cConstant GL_VERTEX_ARRAY GL_COLOR_ARRAY GL_NORMAL_ARRAY
syn keyword cConstant GL_SECONDARY_COLOR_ARRAY GL_INDEX_ARRAY
syn keyword cConstant GL_FOG_COORDINATE_ARRAY GL_TEXTURE_COORD_ARRAY
syn keyword cConstant GL_EDGE_FLAG_ARRAY
syn keyword cConstant GL_EDGE_FLAG_ARRAY GL_VERTEX_ARRAY_SIZE 
syn keyword cConstant GL_VERTEX_ARRAY_TYPE GL_NORMAL_ARRAY_TYPE 
syn keyword cConstant GL_COLOR_ARRAY_SIZE GL_COLOR_ARRAY_TYPE 
syn keyword cConstant GL_COLOR_ARRAY_STRIDE GL_INDEX_ARRAY_TYPE 
syn keyword cConstant GL_INDEX_ARRAY_STRIDE GL_T2F_C4F_N3F_V3F 
syn keyword cConstant GL_T4F_C4F_N3F_V4F GL_MODELVIEW GL_PROJECTION GL_TEXTURE 
syn keyword cConstant GL_POINT_SMOOTH GL_POINT_SIZE GL_POINT_SIZE_RANGE 
syn keyword cConstant GL_LINE_SMOOTH GL_LINE_STIPPLE GL_LINE_STIPPLE_PATTERN 
syn keyword cConstant GL_LINE_STIPPLE_REPEAT GL_LINE_WIDTH GL_LINE_WIDTH_RANGE 
syn keyword cConstant GL_POINT GL_LINE GL_FILL GL_CW GL_CCW GL_FRONT GL_BACK 
syn keyword cConstant GL_POLYGON_MODE GL_POLYGON_SMOOTH GL_POLYGON_STIPPLE 
syn keyword cConstant GL_EDGE_FLAG GL_CULL_FACE GL_CULL_FACE_MODE GL_FRONT_FACE 
syn keyword cConstant GL_POLYGON_OFFSET_UNITS GL_POLYGON_OFFSET_POINT 
syn keyword cConstant GL_POLYGON_OFFSET_LINE GL_POLYGON_OFFSET_FILL GL_COMPILE 
syn keyword cConstant GL_LIST_BASE GL_LIST_INDEX GL_LIST_MODE GL_NEVER GL_LESS 
syn keyword cConstant GL_EQUAL GL_LEQUAL GL_GREATER GL_NOTEQUAL GL_GEQUAL GL_ALWAYS 
syn keyword cConstant GL_DEPTH_TEST GL_DEPTH_BITS GL_DEPTH_FUNC GL_DEPTH_RANGE 
syn keyword cConstant GL_DEPTH_WRITEMASK GL_DEPTH_COMPONENT GL_LIGHTING GL_LIGHT0 
syn keyword cConstant GL_LIGHT1 GL_LIGHT2 GL_LIGHT3 GL_LIGHT4 GL_LIGHT5 GL_LIGHT6 
syn keyword cConstant GL_LIGHT7 GL_SPOT_EXPONENT GL_SPOT_CUTOFF 
syn keyword cConstant GL_CONSTANT_ATTENUATION GL_LINEAR_ATTENUATION 
syn keyword cConstant GL_QUADRATIC_ATTENUATION GL_AMBIENT GL_DIFFUSE GL_SPECULAR 
syn keyword cConstant GL_SHININESS GL_EMISSION GL_POSITION GL_SPOT_DIRECTION 
syn keyword cConstant GL_AMBIENT_AND_DIFFUSE GL_COLOR_INDEXES 
syn keyword cConstant GL_LIGHT_MODEL_TWO_SIDE GL_LIGHT_MODEL_AMBIENT 
syn keyword cConstant GL_FRONT_AND_BACK GL_SHADE_MODEL GL_FLAT GL_SMOOTH 
syn keyword cConstant GL_COLOR_MATERIAL GL_COLOR_MATERIAL_FACE GL_NORMALIZE 
syn keyword cConstant GL_ACCUM_RED_BITS GL_ACCUM_GREEN_BITS GL_ACCUM_BLUE_BITS 
syn keyword cConstant GL_ACCUM_ALPHA_BITS GL_ACCUM GL_ADD GL_LOAD GL_MULT GL_RETURN 
syn keyword cConstant GL_ALPHA_TEST GL_ALPHA_TEST_REF GL_ALPHA_TEST_FUNC GL_BLEND 
syn keyword cConstant GL_BLEND_SRC GL_BLEND_DST GL_ZERO GL_ONE GL_SRC_COLOR 
syn keyword cConstant GL_ONE_MINUS_SRC_COLOR GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA 
syn keyword cConstant GL_DST_ALPHA GL_ONE_MINUS_DST_ALPHA GL_DST_COLOR 
syn keyword cConstant GL_ONE_MINUS_DST_COLOR GL_SRC_ALPHA_SATURATE 
syn keyword cConstant GL_CONSTANT_COLOR GL_CONSTANT_ALPHA GL_RENDER GL_SELECT 
syn keyword cConstant GL_2D GL_3D GL_3D_COLOR GL_3D_COLOR_TEXTURE 
syn keyword cConstant GL_4D_COLOR_TEXTURE GL_POINT_TOKEN GL_LINE_TOKEN 
syn keyword cConstant GL_LINE_RESET_TOKEN GL_POLYGON_TOKEN GL_BITMAP_TOKEN 
syn keyword cConstant GL_DRAW_PIXEL_TOKEN GL_COPY_PIXEL_TOKEN 
syn keyword cConstant GL_PASS_THROUGH_TOKEN GL_FEEDBACK_BUFFER_SIZE 
syn keyword cConstant GL_FEEDBACK_BUFFER_TYPE GL_SELECTION_BUFFER_SIZE GL_FOG 
syn keyword cConstant GL_FOG_MODE GL_FOG_COLOR GL_FOG_INDEX GL_FOG_START 
syn keyword cConstant GL_FOG_END GL_LINEAR GL_EXP GL_EXP2 GL_LOGIC_OP 
syn keyword cConstant GL_LOGIC_OP_MODE GL_CLEAR GL_SET GL_COPY GL_COPY_INVERTED 
syn keyword cConstant GL_NOOP GL_INVERT GL_AND GL_NAND GL_OR GL_NOR GL_XOR GL_EQUIV 
syn keyword cConstant GL_AND_REVERSE GL_AND_INVERTED GL_OR_REVERSE GL_OR_INVERTED 
syn keyword cConstant GL_STENCIL_TEST GL_STENCIL_WRITEMASK GL_STENCIL_BITS 
syn keyword cConstant GL_STENCIL_FUNC GL_STENCIL_VALUE_MASK GL_STENCIL_REF 
syn keyword cConstant GL_STENCIL_FAIL GL_STENCIL_CLEAR_VALUE GL_STENCIL_INDEX 
syn keyword cConstant GL_KEEP GL_REPLACE GL_INCR GL_DECR GL_NONE GL_LEFT GL_RIGHT 
syn keyword cConstant GL_FRONT_LEFT GL_FRONT_RIGHT GL_BACK_LEFT GL_BACK_RIGHT 
syn keyword cConstant GL_AUX0 GL_AUX1 GL_AUX2 GL_AUX3 GL_COLOR_INDEX GL_RED GL_GREEN 
syn keyword cConstant GL_BLUE GL_ALPHA GL_LUMINANCE GL_ALPHA_BITS GL_RED_BITS 
syn keyword cConstant GL_GREEN_BITS GL_BLUE_BITS GL_INDEX_BITS GL_AUX_BUFFERS 
syn keyword cConstant GL_READ_BUFFER GL_DRAW_BUFFER GL_STEREO GL_BITMAP GL_COLOR 
syn keyword cConstant GL_DEPTH GL_STENCIL GL_DITHER GL_RGB GL_RGBA 
syn keyword cConstant GL_MAX_LIST_NESTING GL_MAX_ATTRIB_STACK_DEPTH 
syn keyword cConstant GL_MAX_MODELVIEW_STACK_DEPTH GL_MAX_NAME_STACK_DEPTH 
syn keyword cConstant GL_MAX_PROJECTION_STACK_DEPTH GL_MAX_TEXTURE_STACK_DEPTH 
syn keyword cConstant GL_MAX_EVAL_ORDER GL_MAX_LIGHTS GL_MAX_CLIP_PLANES 
syn keyword cConstant GL_MAX_TEXTURE_SIZE GL_MAX_PIXEL_MAP_TABLE 
syn keyword cConstant GL_MAX_VIEWPORT_DIMS GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 
syn keyword cConstant GL_ATTRIB_STACK_DEPTH GL_CLIENT_ATTRIB_STACK_DEPTH 
syn keyword cConstant GL_COLOR_CLEAR_VALUE GL_COLOR_WRITEMASK GL_CURRENT_INDEX 
syn keyword cConstant GL_CURRENT_COLOR GL_CURRENT_NORMAL GL_CURRENT_RASTER_COLOR 
syn keyword cConstant GL_CURRENT_RASTER_DISTANCE GL_CURRENT_RASTER_INDEX 
syn keyword cConstant GL_CURRENT_RASTER_POSITION GL_CURRENT_TEXTURE_COORDS 
syn keyword cConstant GL_INDEX_CLEAR_VALUE GL_INDEX_MODE GL_INDEX_WRITEMASK 
syn keyword cConstant GL_MODELVIEW_MATRIX GL_MODELVIEW_STACK_DEPTH 
syn keyword cConstant GL_NAME_STACK_DEPTH GL_PROJECTION_MATRIX 
syn keyword cConstant GL_PROJECTION_STACK_DEPTH GL_RENDER_MODE GL_RGBA_MODE 
syn keyword cConstant GL_TEXTURE_MATRIX GL_TEXTURE_STACK_DEPTH GL_VIEWPORT 
syn keyword cConstant GL_AUTO_NORMAL GL_MAP1_COLOR_4 GL_MAP1_GRID_DOMAIN 
syn keyword cConstant GL_MAP1_GRID_SEGMENTS GL_MAP1_INDEX GL_MAP1_NORMAL 
syn keyword cConstant GL_MAP1_VERTEX_3 GL_MAP1_VERTEX_4 GL_MAP2_COLOR_4 
syn keyword cConstant GL_MAP2_GRID_DOMAIN GL_MAP2_GRID_SEGMENTS GL_MAP2_INDEX 
syn keyword cConstant GL_MAP2_NORMAL GL_MAP2_VERTEX_3 GL_MAP2_VERTEX_4 GL_COEFF 
syn keyword cConstant GL_DOMAIN GL_ORDER GL_FOG_HINT GL_LINE_SMOOTH_HINT 
syn keyword cConstant GL_POINT_SMOOTH_HINT GL_POLYGON_SMOOTH_HINT GL_DONT_CARE 
syn keyword cConstant GL_FASTEST GL_NICEST GL_SCISSOR_BOX GL_MAP_COLOR 
syn keyword cConstant GL_MAP_STENCIL GL_INDEX_SHIFT GL_INDEX_OFFSET GL_RED_SCALE 
syn keyword cConstant GL_RED_BIAS GL_GREEN_SCALE GL_GREEN_BIAS GL_BLUE_SCALE 
syn keyword cConstant GL_BLUE_BIAS GL_ALPHA_SCALE GL_ALPHA_BIAS GL_DEPTH_SCALE 
syn keyword cConstant GL_DEPTH_BIAS GL_PIXEL_MAP_S_TO_S GL_PIXEL_MAP_I_TO_I 
syn keyword cConstant GL_PIXEL_MAP_I_TO_R GL_PIXEL_MAP_I_TO_G 
syn keyword cConstant GL_PIXEL_MAP_I_TO_B GL_PIXEL_MAP_I_TO_A 
syn keyword cConstant GL_PIXEL_MAP_R_TO_R GL_PIXEL_MAP_G_TO_G 
syn keyword cConstant GL_PIXEL_MAP_B_TO_B GL_PIXEL_MAP_A_TO_A GL_PACK_ALIGNMENT 
syn keyword cConstant GL_PACK_LSB_FIRST GL_PACK_ROW_LENGTH GL_PACK_SKIP_PIXELS 
syn keyword cConstant GL_PACK_SKIP_ROWS GL_PACK_SWAP_BYTES GL_UNPACK_ALIGNMENT 
syn keyword cConstant GL_UNPACK_ROW_LENGTH GL_UNPACK_SKIP_PIXELS 
syn keyword cConstant GL_UNPACK_SKIP_ROWS GL_UNPACK_SWAP_BYTES GL_ZOOM_X 
syn keyword cConstant GL_ZOOM_Y GL_TEXTURE_ENV GL_TEXTURE_ENV_MODE GL_TEXTURE_1D 
syn keyword cConstant GL_TEXTURE_2D GL_TEXTURE_WRAP_S GL_TEXTURE_WRAP_T 
syn keyword cConstant GL_TEXTURE_MAG_FILTER GL_TEXTURE_MIN_FILTER 
syn keyword cConstant GL_TEXTURE_ENV_COLOR GL_TEXTURE_GEN_S GL_TEXTURE_GEN_T 
syn keyword cConstant GL_TEXTURE_GEN_MODE GL_TEXTURE_BORDER_COLOR 
syn keyword cConstant GL_TEXTURE_WIDTH GL_TEXTURE_HEIGHT GL_TEXTURE_BORDER 
syn keyword cConstant GL_TEXTURE_COMPONENTS GL_TEXTURE_RED_SIZE 
syn keyword cConstant GL_TEXTURE_GREEN_SIZE GL_TEXTURE_BLUE_SIZE 
syn keyword cConstant GL_TEXTURE_ALPHA_SIZE GL_NEAREST_MIPMAP_LINEAR 
syn keyword cConstant GL_LINEAR_MIPMAP_NEAREST GL_LINEAR_MIPMAP_LINEAR 
syn keyword cConstant GL_OBJECT_LINEAR GL_OBJECT_PLANE GL_EYE_LINEAR GL_EYE_PLANE 
syn keyword cConstant GL_SPHERE_MAP GL_DECAL GL_MODULATE GL_NEAREST GL_REPEAT 
syn keyword cConstant GL_CLAMP GL_S GL_T GL_R GL_Q GL_TEXTURE_GEN_R GL_TEXTURE_GEN_Q 
syn keyword cConstant GL_VENDOR GL_RENDERER GL_VERSION GL_NO_ERROR 
syn keyword cConstant GL_INVALID_VALUE GL_INVALID_ENUM GL_STACK_OVERFLOW 
syn keyword cConstant GL_STACK_UNDERFLOW GL_OUT_OF_MEMORY GL_CURRENT_BIT 
syn keyword cConstant GL_POINT_BIT GL_LINE_BIT GL_POLYGON_BIT GL_PIXEL_MODE_BIT 
syn keyword cConstant GL_LIGHTING_BIT GL_FOG_BIT GL_DEPTH_BUFFER_BIT 
syn keyword cConstant GL_ACCUM_BUFFER_BIT GL_STENCIL_BUFFER_BIT GL_VIEWPORT_BIT 
syn keyword cConstant GL_TRANSFORM_BIT GL_ENABLE_BIT GL_COLOR_BUFFER_BIT 
syn keyword cConstant GL_HINT_BIT GL_EVAL_BIT GL_LIST_BIT GL_TEXTURE_BIT 
syn keyword cConstant GL_SCISSOR_BIT GL_ALL_ATTRIB_BITS GL_PROXY_TEXTURE_1D 
syn keyword cConstant GL_PROXY_TEXTURE_2D GL_TEXTURE_PRIORITY 
syn keyword cConstant GL_TEXTURE_RESIDENT GL_TEXTURE_BINDING_1D 
syn keyword cConstant GL_TEXTURE_BINDING_2D GL_ALPHA4 GL_ALPHA8 GL_ALPHA12 
syn keyword cConstant GL_ALPHA16 GL_LUMINANCE4 GL_LUMINANCE8 GL_LUMINANCE12 
syn keyword cConstant GL_LUMINANCE16 GL_LUMINANCE4_ALPHA4 GL_LUMINANCE6_ALPHA2 
syn keyword cConstant GL_LUMINANCE8_ALPHA8 GL_LUMINANCE12_ALPHA4 
syn keyword cConstant GL_LUMINANCE12_ALPHA12 GL_LUMINANCE16_ALPHA16 GL_INTENSITY 
syn keyword cConstant GL_INTENSITY4 GL_INTENSITY8 GL_INTENSITY12 GL_INTENSITY16 
syn keyword cConstant GL_R3_G3_B2 GL_RGB4 GL_RGB5 GL_RGB8 GL_RGB10 GL_RGB12 GL_RGB16 
syn keyword cConstant GL_RGBA2 GL_RGBA4 GL_RGB5_A1 GL_RGBA8 GL_RGB10_A2 GL_RGBA12 
syn keyword cConstant GL_RGBA16 GL_CLIENT_PIXEL_STORE_BIT 
syn keyword cConstant GL_ALL_CLIENT_ATTRIB_BITS GL_CLIENT_ALL_ATTRIB_BITS 
syn keyword cConstant GL_RESCALE_NORMAL GL_CLAMP_TO_EDGE 
syn keyword cConstant GL_MAX_ELEMENTS_VERTICES GL_MAX_ELEMENTS_INDICES GL_BGR 
syn keyword cConstant GL_BGRA GL_UNSIGNED_BYTE_3_3_2 GL_UNSIGNED_BYTE_2_3_3_REV 
syn keyword cConstant GL_UNSIGNED_SHORT_5_6_5 GL_UNSIGNED_SHORT_5_6_5_REV 
syn keyword cConstant GL_UNSIGNED_SHORT_4_4_4_4 GL_UNSIGNED_SHORT_4_4_4_4_REV 
syn keyword cConstant GL_UNSIGNED_SHORT_5_5_5_1 GL_UNSIGNED_SHORT_1_5_5_5_REV 
syn keyword cConstant GL_UNSIGNED_INT_8_8_8_8 GL_UNSIGNED_INT_8_8_8_8_REV 
syn keyword cConstant GL_UNSIGNED_INT_10_10_10_2 GL_UNSIGNED_INT_2_10_10_10_REV 
syn keyword cConstant GL_LIGHT_MODEL_COLOR_CONTROL GL_SINGLE_COLOR 
syn keyword cConstant GL_SEPARATE_SPECULAR_COLOR GL_TEXTURE_MIN_LOD 
syn keyword cConstant GL_TEXTURE_MAX_LOD GL_TEXTURE_BASE_LEVEL 
syn keyword cConstant GL_TEXTURE_MAX_LEVEL GL_SMOOTH_POINT_SIZE_RANGE 
syn keyword cConstant GL_SMOOTH_LINE_WIDTH_RANGE GL_ALIASED_POINT_SIZE_RANGE 
syn keyword cConstant GL_ALIASED_LINE_WIDTH_RANGE GL_PACK_SKIP_IMAGES 
syn keyword cConstant GL_PACK_IMAGE_HEIGHT GL_UNPACK_SKIP_IMAGES 
syn keyword cConstant GL_UNPACK_IMAGE_HEIGHT GL_TEXTURE_3D GL_PROXY_TEXTURE_3D 
syn keyword cConstant GL_TEXTURE_DEPTH GL_TEXTURE_WRAP_R GL_MAX_3D_TEXTURE_SIZE 
syn keyword cConstant GL_TEXTURE_BINDING_3D GL_TEXTURE0 GL_TEXTURE1 GL_TEXTURE2 
syn keyword cConstant GL_TEXTURE3 GL_TEXTURE4 GL_TEXTURE5 GL_TEXTURE6 GL_TEXTURE7 
syn keyword cConstant GL_TEXTURE8 GL_TEXTURE9 GL_TEXTURE10 GL_TEXTURE11 
syn keyword cConstant GL_TEXTURE12 GL_TEXTURE13 GL_TEXTURE14 GL_TEXTURE15 
syn keyword cConstant GL_TEXTURE16 GL_TEXTURE17 GL_TEXTURE18 GL_TEXTURE19 
syn keyword cConstant GL_TEXTURE20 GL_TEXTURE21 GL_TEXTURE22 GL_TEXTURE23 
syn keyword cConstant GL_TEXTURE24 GL_TEXTURE25 GL_TEXTURE26 GL_TEXTURE27 
syn keyword cConstant GL_TEXTURE28 GL_TEXTURE29 GL_TEXTURE30 GL_TEXTURE31 
syn keyword cConstant GL_ACTIVE_TEXTURE GL_CLIENT_ACTIVE_TEXTURE 
syn keyword cConstant GL_MAX_TEXTURE_UNITS GL_NORMAL_MAP GL_REFLECTION_MAP 
syn keyword cConstant GL_TEXTURE_CUBE_MAP GL_TEXTURE_BINDING_CUBE_MAP 
syn keyword cConstant GL_TEXTURE_CUBE_MAP_POSITIVE_X 
syn keyword cConstant GL_TEXTURE_CUBE_MAP_NEGATIVE_X 
syn keyword cConstant GL_TEXTURE_CUBE_MAP_POSITIVE_Y 
syn keyword cConstant GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 
syn keyword cConstant GL_TEXTURE_CUBE_MAP_POSITIVE_Z 
syn keyword cConstant GL_TEXTURE_CUBE_MAP_NEGATIVE_Z GL_PROXY_TEXTURE_CUBE_MAP 
syn keyword cConstant GL_MAX_CUBE_MAP_TEXTURE_SIZE GL_COMPRESSED_ALPHA 
syn keyword cConstant GL_COMPRESSED_LUMINANCE GL_COMPRESSED_LUMINANCE_ALPHA 
syn keyword cConstant GL_COMPRESSED_INTENSITY GL_COMPRESSED_RGB 
syn keyword cConstant GL_COMPRESSED_RGBA GL_TEXTURE_COMPRESSION_HINT 
syn keyword cConstant GL_TEXTURE_COMPRESSED_IMAGE_SIZE GL_TEXTURE_COMPRESSED 
syn keyword cConstant GL_COMPRESSED_TEXTURE_FORMATS GL_MULTISAMPLE 
syn keyword cConstant GL_SAMPLE_ALPHA_TO_COVERAGE GL_SAMPLE_ALPHA_TO_ONE 
syn keyword cConstant GL_SAMPLE_COVERAGE GL_SAMPLE_BUFFERS GL_SAMPLES 
syn keyword cConstant GL_SAMPLE_COVERAGE_VALUE GL_SAMPLE_COVERAGE_INVERT 
syn keyword cConstant GL_MULTISAMPLE_BIT GL_TRANSPOSE_MODELVIEW_MATRIX 
syn keyword cConstant GL_TRANSPOSE_PROJECTION_MATRIX 
syn keyword cConstant GL_TRANSPOSE_TEXTURE_MATRIX GL_TRANSPOSE_COLOR_MATRIX 
syn keyword cConstant GL_COMBINE GL_COMBINE_RGB GL_COMBINE_ALPHA GL_SOURCE0_RGB 
syn keyword cConstant GL_SOURCE1_RGB GL_SOURCE2_RGB GL_SOURCE0_ALPHA 
syn keyword cConstant GL_SOURCE1_ALPHA GL_SOURCE2_ALPHA GL_OPERAND0_RGB 
syn keyword cConstant GL_OPERAND1_RGB GL_OPERAND2_RGB GL_OPERAND0_ALPHA 
syn keyword cConstant GL_OPERAND1_ALPHA GL_OPERAND2_ALPHA GL_RGB_SCALE 
syn keyword cConstant GL_ADD_SIGNED GL_INTERPOLATE GL_SUBTRACT GL_CONSTANT 
syn keyword cConstant GL_PRIMARY_COLOR GL_PREVIOUS GL_DOT3_RGB GL_DOT3_RGBA 
syn keyword cConstant GL_CLAMP_TO_BORDER GL_TEXTURE0_ARB GL_TEXTURE1_ARB 
syn keyword cConstant GL_TEXTURE2_ARB GL_TEXTURE3_ARB GL_TEXTURE4_ARB 
syn keyword cConstant GL_TEXTURE5_ARB GL_TEXTURE6_ARB GL_TEXTURE7_ARB 
syn keyword cConstant GL_TEXTURE8_ARB GL_TEXTURE9_ARB GL_TEXTURE10_ARB 
syn keyword cConstant GL_TEXTURE11_ARB GL_TEXTURE12_ARB GL_TEXTURE13_ARB 
syn keyword cConstant GL_TEXTURE14_ARB GL_TEXTURE15_ARB GL_TEXTURE16_ARB 
syn keyword cConstant GL_TEXTURE17_ARB GL_TEXTURE18_ARB GL_TEXTURE19_ARB 
syn keyword cConstant GL_TEXTURE20_ARB GL_TEXTURE21_ARB GL_TEXTURE22_ARB 
syn keyword cConstant GL_TEXTURE23_ARB GL_TEXTURE24_ARB GL_TEXTURE25_ARB 
syn keyword cConstant GL_TEXTURE26_ARB GL_TEXTURE27_ARB GL_TEXTURE28_ARB 
syn keyword cConstant GL_TEXTURE29_ARB GL_TEXTURE30_ARB GL_TEXTURE31_ARB 
syn keyword cConstant GL_ACTIVE_TEXTURE_ARB GL_MAX_TEXTURE_UNITS_ARB 
syn keyword cConstant GL_ARB_VERTEX_PROGRAM GL_POINTS GL_LINES GL_LINE_STRIP
syn keyword cConstant GL_LINE_LOOP GL_TRIANGLES GL_TRIANGLE_STRIP GL_TRIANGLE_FAN
syn keyword cConstant GL_QUADS GL_QUAD_STRIP GL_POLYGON GL_ARRAY_BUFFER
syn keyword cConstant GL_ELEMENT_ARRAY_BUFFER GL_STREAM_DRAW GL_STREAM_READ
syn keyword cConstant GL_STREAM_COPY GL_STATIC_DRAW GL_STATIC_READ GL_STATIC_COPY
syn keyword cConstant GL_DYNAMIC_DRAW GL_DYNAMIC_READ GL_DYNAMIC_COPY

" Accept %: for # (C99)
syn region      cPreCondit      start="^\s*\(%:\|#\)\s*\(if\|ifdef\|ifndef\|elif\)\>" skip="\\$" end="$"  keepend contains=cComment,cCommentL,cCppString,cCharacter,cCppParen,cParenError,cNumbers,cCommentError,cSpaceError
syn match	cPreCondit	display "^\s*\(%:\|#\)\s*\(else\|endif\)\>"
if !exists("c_no_if0")
  if !exists("c_no_if0_fold")
    syn region	cCppOut		start="^\s*\(%:\|#\)\s*if\s\+0\+\>" end=".\@=\|$" contains=cCppOut2 fold
  else
    syn region	cCppOut		start="^\s*\(%:\|#\)\s*if\s\+0\+\>" end=".\@=\|$" contains=cCppOut2
  endif
  syn region	cCppOut2	contained start="0" end="^\s*\(%:\|#\)\s*\(endif\>\|else\>\|elif\>\)" contains=cSpaceError,cCppSkip
  syn region	cCppSkip	contained start="^\s*\(%:\|#\)\s*\(if\>\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" contains=cSpaceError,cCppSkip
endif
syn region	cIncluded	display contained start=+"+ skip=+\\\\\|\\"+ end=+"+
syn match	cIncluded	display contained "<[^>]*>"
syn match	cInclude	display "^\s*\(%:\|#\)\s*include\>\s*["<]" contains=cIncluded
"syn match cLineSkip	"\\$"
syn cluster	cPreProcGroup	contains=cPreCondit,cIncluded,cInclude,cDefine,cErrInParen,cErrInBracket,cUserLabel,cSpecial,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom,cString,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cParen,cBracket,cMulti
syn region	cDefine		start="^\s*\(%:\|#\)\s*\(define\|undef\)\>" skip="\\$" end="$" keepend contains=ALLBUT,@cPreProcGroup,@Spell
syn region	cPreProc	start="^\s*\(%:\|#\)\s*\(pragma\>\|line\>\|warning\>\|warn\>\|error\>\)" skip="\\$" end="$" keepend contains=ALLBUT,@cPreProcGroup,@Spell

" Highlight User Labels
syn cluster	cMultiGroup	contains=cIncluded,cSpecial,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cUserCont,cUserLabel,cBitField,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom,cCppParen,cCppBracket,cCppString
syn region	cMulti		transparent start='?' skip='::' end=':' contains=ALLBUT,@cMultiGroup,@Spell
" Avoid matching foo::bar() in C++ by requiring that the next char is not ':'
syn cluster	cLabelGroup	contains=cUserLabel
syn match	cUserCont	display "^\s*\I\i*\s*:$" contains=@cLabelGroup
syn match	cUserCont	display ";\s*\I\i*\s*:$" contains=@cLabelGroup
syn match	cUserCont	display "^\s*\I\i*\s*:[^:]"me=e-1 contains=@cLabelGroup
syn match	cUserCont	display ";\s*\I\i*\s*:[^:]"me=e-1 contains=@cLabelGroup

syn match	cUserLabel	display "\I\i*" contained

" Avoid recognizing most bitfields as labels
syn match	cBitField	display "^\s*\I\i*\s*:\s*[1-9]"me=e-1 contains=cType
syn match	cBitField	display ";\s*\I\i*\s*:\s*[1-9]"me=e-1 contains=cType

if exists("c_minlines")
  let b:c_minlines = c_minlines
else
  if !exists("c_no_if0")
    let b:c_minlines = 50	" #if 0 constructs can be long
  else
    let b:c_minlines = 15	" mostly for () constructs
  endif
endif
if exists("c_curly_error")
  syn sync fromstart
else
  exec "syn sync ccomment cComment minlines=" . b:c_minlines
endif

" Define the default highlighting.
" Only used when an item doesn't have highlighting yet
hi def link cFormat		cSpecial
hi def link cCppString		cString
hi def link cCommentL		cComment
hi def link cCommentStart	cComment
hi def link cLabel		Label
hi def link cUserLabel		Label
hi def link cConditional	Conditional
hi def link cRepeat		Repeat
hi def link cCharacter		Character
hi def link cSpecialCharacter	cSpecial
hi def link cNumber		Number
hi def link cOctal		Number
hi def link cOctalZero		PreProc	 " link this to Error if you want
hi def link cFloat		Float
hi def link cOctalError		cError
hi def link cParenError		cError
hi def link cErrInParen		cError
hi def link cErrInBracket	cError
hi def link cCommentError	cError
hi def link cCommentStartError	cError
hi def link cSpaceError		cError
hi def link cSpecialError	cError
hi def link cCurlyError		cError
hi def link cOperator		Operator
hi def link cStructure		Structure
hi def link cStorageClass	StorageClass
hi def link cInclude		Include
hi def link cPreProc		PreProc
hi def link cDefine		Macro
hi def link cIncluded		cString
hi def link cError		Error
hi def link cStatement		Statement
hi def link cPreCondit		PreCondit
hi def link cType		Type
hi def link cConstant		Constant
hi def link cCommentString	cString
hi def link cComment2String	cString
hi def link cCommentSkip	cComment
hi def link cString		String
hi def link cComment		Comment
hi def link cSpecial		SpecialChar
hi def link cTodo		Todo
hi def link cBadContinuation	Error
hi def link cCppSkip		cCppOut
hi def link cCppOut2		cCppOut
hi def link cCppOut		Comment

let b:current_syntax = "c"

" vim: ts=8
