#ifndef attr_reader_val
#define attr_reader_val(_var, _func)  \
  inline auto _func() const { return _var; }
#endif

#ifndef attr_reader_ref
#define attr_reader_ref(_var, _func)  \
  inline auto &&_func() const { return _var; }
#endif
