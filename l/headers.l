(include std)
(include c)

(defns headers)

(def (addid headername lid cid)
  (bind headers lid ((O Identifier new) (str cid) nil headername)))

(devau (defh headername . ids) env
  (map1 [addid headername _ _] ids))

(devau (defh2 headername . ids) env
  (each2 ids (fn (lid cid) (addid headername lid cid))))

(defh2 stdlib null NULL)
(defh stdlib
  size_t atexit exit free calloc malloc)

(defh2 stdint
  s8 int8_t
  s16 int16_t
  s32 int32_t
  s64 int64_t
  u8 uint8_t
  u16 uint16_t
  u32 uint32_t
  u64 uint64_t)

(defh stdbool bool true false)

(defh stdio
  stdin stdout stderr FILE feof fgetc fprintf)

(defh string
  memcpy memcmp)

(defh stdarg
  va_list va_start va_copy va_end vsnprintf)

(def stdc (+ C headers))

(export stdc)
