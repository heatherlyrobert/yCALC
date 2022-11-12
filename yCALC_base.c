/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"

/*
 * metis § ····· § trig formula for line of site distance above arc to end points         § M453JT §  · §
 * metis § ····· § exsag is from arc to line of site height to end points                 § M453K8 §  · §
 * metis § ····· § los (line of sight) is distance one can see given height and radius    § M453LC §  · §
 *
 */


tLOCAL      myCALC;

double      a, b, c, d, e;
int         s_buf, m, n, o, len;
char       *q, *r, *s;
char        t           [LEN_RECD];
char        g_nada       [5] = "";



const tFCAT s_fcats  [MAX_FCAT] = {
   { '-', "unknown grouping"                      },
   { 'm', "arithmetic and basic mathmatics"       },
   { 'l', "logic and relational comparisons"      },
   { 's', "string production and manipulation"    },
   { 'c', "string conversions and cleaning"       },
   { 't', "trigonometry and geometry"             },
   { 'a', "cell addresses and ranges"             },
   { 'i', "cell information and debugging"        },
   { 'r', "range information and statistics"      },
   { 'd', "date and time manipulation"            },
   { 'f', "lookup and finding range data"         },
   { 'b', "bitwise manipulations"                 },
   { ' ', "end-list"                              },
};


const tFUNCS  g_ycalc_funcs [MAX_FUNCS] = {
   /*---(mathmatical operators)-----------*/
   { "+"          ,  1, ycalc_add               , 'o', "n:nn"   , 'm', "ansi-c addition"                                   },
   { "-"          ,  1, ycalc_subtract          , 'o', "n:nn"   , 'm', "ansi-c subtraction"                                },
   { "*"          ,  1, ycalc_multiply          , 'o', "n:nn"   , 'm', "ansi-c multipication"                              },
   { "/"          ,  1, ycalc_divide            , 'o', "n:nn"   , 'm', "ansi-c division"                                   },
   { "%"          ,  1, ycalc_modulus           , 'o', "n:nn"   , 'm', "ansi-c modulus"                                    },
   { "++"         ,  2, ycalc_increment         , 'o', "n:n"    , 'm', "truly x + 1 as base variable uneffected"           },
   { "--"         ,  2, ycalc_decrement         , 'o', "n:n"    , 'm', "truly x - 1 as base variable uneffected"           },
   { ":+"         ,  2, ycalc_increment         , 'o', "n:n"    , 'm', "non-standard, acts like prefix ++"                 },
   { ":-"         ,  2, ycalc_decrement         , 'o', "n:n"    , 'm', "non-standard, acts like prefix --"                 },
   { "-:"         ,  2, ycalc_unaryminus        , 'o', "n:n"    , 'm', "ansi-c unary minus"                                },
   { "+:"         ,  2, ycalc_noop              , 'o', "n:n"    , 'm', "ansi-c unary plus (no effect)"                     },
   /*---(mathmatical functions)-----------*/
   { "exp"        ,  3, ycalc_power             , 'f', "n:nn"   , 'm', "x raised to the power of y"                        },
   { "Æ"          ,  1, ycalc_power_of_2        , 'f', "n:n"    , 'm', "x raised to the power of 2"                        },
   { "Ç"          ,  1, ycalc_power_of_3        , 'f', "n:n"    , 'm', "x raised to the power of 3"                        },
   { "È"          ,  1, ycalc_power_of_4        , 'f', "n:n"    , 'm', "x raised to the power of 4"                        },
   { "É"          ,  1, ycalc_power_of_x        , 'f', "n:n"    , 'm', "x raised to the power of 4"                        },
   { "Ê"          ,  1, ycalc_power_of_y        , 'f', "n:n"    , 'm', "x raised to the power of 4"                        },
   { "abs"        ,  3, ycalc_abs               , 'f', "n:n"    , 'm', "ansi-c fabs() removes negative sign"               },
   { "sign"       ,  4, ycalc_sign              , 'f', "n:n"    , 'm', "neg = -1, pos = +1, zero = 0"                      },
   { "trunc"      ,  5, ycalc_trunc             , 'f', "n:n"    , 'm', "truncate to the nearest integer value"             },
   { "truncn"     ,  6, ycalc_rtrunc            , 'f', "n:nn"   , 'm', "truncate to y decimal places"                      },
   { "round"      ,  5, ycalc_round             , 'f', "n:n"    , 'm', "round, up or down, to the nearest integer"         },
   { "roundn"     ,  6, ycalc_rround            , 'f', "n:nn"   , 'm', "round, up or down, to y decimal places"            },
   { "ceil"       ,  4, ycalc_ceiling           , 'f', "n:n"    , 'm', "raise up to the nearest integer"                   },
   { "floor"      ,  5, ycalc_floor             , 'f', "n:n"    , 'm', "push down to the nearest integer"                  },
   { "sqrt"       ,  4, ycalc_sqrt              , 'f', "n:n"    , 'm', "ansi-c square root"                                },
   { "Î"          ,  1, ycalc_sqrt              , 'f', "n:n"    , 'm', "ansi-c square root"                                },
   { "cbrt"       ,  4, ycalc_cbrt              , 'f', "n:n"    , 'm', "cube root of x"                                    },
   { "sqr"        ,  3, ycalc_power_of_2        , 'f', "n:n"    , 'm', "square of x"                                       },
   { "cube"       ,  4, ycalc_power_of_3        , 'f', "n:n"    , 'm', "cube of x"                                         },
   { "rand"       ,  4, ycalc_rand              , 'f', "n:"     , 'm', "random number betreen 0 and 1"                     },
   { "randr"      ,  5, ycalc_randr             , 'f', "n:nn"   , 'm', "random number betreen x and y"                     },
   /*---(relational operators)------------*/
   { "=="         ,  2, ycalc_equal             , 'o', "t:nn"   , 'l', "T if x are y are equal, else F"                    },
   { "!="         ,  2, ycalc_notequal          , 'o', "t:nn"   , 'l', "T if x not equal y, else F"                        },
   { ">"          ,  1, ycalc_greater           , 'o', "t:nn"   , 'l', "T if x greater than y, else F"                     },
   { "<"          ,  1, ycalc_lesser            , 'o', "t:nn"   , 'l', "T if x lesser than y, else F"                      },
   { ">="         ,  2, ycalc_gequal            , 'o', "t:nn"   , 'l', "T if x greater than or equal to y, else F"         },
   { "<="         ,  2, ycalc_lequal            , 'o', "t:nn"   , 'l', "T if x lesser than or equal to y, else F"          },
   { "›="         ,  2, ycalc_sequal            , 'o', "t:ss"   , 'l', "T if n are m are equal, else F"                    },
   { "›!"         ,  2, ycalc_snotequal         , 'o', "t:ss"   , 'l', "T if n not equal m, else F"                        },
   { "›<"         ,  2, ycalc_slesser           , 'o', "t:ss"   , 'l', "T if n greater than m, else F"                     },
   { "›>"         ,  2, ycalc_sgreater          , 'o', "t:ss"   , 'l', "T if n lesser than m, else F"                      },
   /*---(locgical operators)--------------*/
   { "!"          ,  1, ycalc_not               , 'o', "t:n"    , 'l', "T if x F, else F"                                  },
   { "&&"         ,  2, ycalc_and               , 'o', "t:nn"   , 'l', "T if both x and y are T, else F"                   },
   { "||"         ,  2, ycalc_or                , 'o', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "!&"         ,  2, ycalc_nand              , 'o', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "!|"         ,  2, ycalc_nor               , 'o', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "&|"         ,  2, ycalc_xor               , 'o', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "|&"         ,  2, ycalc_nxor              , 'o', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   /*---(logical functions)---------------*/
   { "if"         ,  2, ycalc_if                , 'f', "n:tnn"  , 'l', "if x is T, y, else z"                              },
   { "ifs"        ,  3, ycalc_ifs               , 'f', "s:tss"  , 'l', "if x is T, n, else m"                              },
   { "within"     ,  6, ycalc_within            , 'f', "t:nnn"  , 'l', "if y is within the range of x to z, then T"        },
   { "approx"     ,  6, ycalc_approx            , 'f', "t:nnn"  , 'l', "if x is within the range of y +/- z, then T"       },
   { "and"        ,  3, ycalc_and               , 'f', "t:nn"   , 'l', "T if both x and y are T, else F"                   },
   { "or"         ,  2, ycalc_or                , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "nand"       ,  4, ycalc_nand              , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "nor"        ,  3, ycalc_nor               , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "xor"        ,  3, ycalc_xor               , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "nxor"       ,  4, ycalc_nxor              , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   /*---(bitwise functions)---------------*/
   { "~"          ,  1, ycalc_bit_not           , 'f', "t:n"    , 'l', "T if either x or y is T, else F"                   },
   { "<<"         ,  2, ycalc_bit_left          , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { ">>"         ,  2, ycalc_bit_right         , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "&"          ,  1, ycalc_bit_and           , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "|"          ,  1, ycalc_bit_or            , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   { "^"          ,  1, ycalc_bit_xor           , 'f', "t:nn"   , 'l', "T if either x or y is T, else F"                   },
   /*---(string operators)----------------*/
   { "›"          ,  1, ycalc_concat            , 'o', "s:ss"   , 's', "m concatinated to the end of n"                    },
   { "››"         ,  2, ycalc_concatplus        , 'o', "s:ss"   , 's', "m concatinated to the end of n (with a space)"     },
   /*---(string functions)----------------*/
   { "len"        ,  3, ycalc_len               , 'f', "n:s"    , 's', "length of n"                                       },
   { "left"       ,  4, ycalc_left              , 'f', "s:sn"   , 's', "left x characters of n"                            },
   { "right"      ,  5, ycalc_right             , 'f', "s:sn"   , 's', "right x characters of n"                           },
   { "mid"        ,  3, ycalc_mid               , 'f', "s:snn"  , 's', "y characters of n, starting at x"                  },
   { "beg"        ,  3, ycalc_beg               , 'f', "s:sn"   , 's', "all characters of n, starting at x"                },
   { "char"       ,  4, ycalc_at                , 'f', "s:sn"   , 's', "the character of n, exactly at x"                  },
   { "trim"       ,  4, ycalc_trim              , 'f', "s:s"    , 's', "trim leading and trailing whitespace from n"       },
   { "ltrim"      ,  5, ycalc_ltrim             , 'f', "s:s"    , 's', "trim just leading whitespace from n"               },
   { "rtrim"      ,  5, ycalc_rtrim             , 'f', "s:s"    , 's', "trim just trailing whitespace from n"              },
   { "strim"      ,  5, ycalc_strim             , 'f', "s:s"    , 's', "compress all whitespce to single, except quoted"   },
   { "etrim"      ,  5, ycalc_etrim             , 'f', "s:s"    , 's', "trim every whitespace in n excluding quoted"       },
   { "mtrim"      ,  5, ycalc_mtrim             , 'f', "s:s"    , 's', "trim every whitespace in n including quoted"       },
   { "print"      ,  5, ycalc_printstr          , 'f', "s:a"    , 's', "gyges trimmed print string of cell a"              },
   { "p"          ,  1, ycalc_printstr          , 'f', "s:a"    , 's', "gyges trimmed print string of cell a"              },
   { "printnum"   ,  8, ycalc_printnum          , 'f', "n:a"    , 's', "gyges trimmed print string as value of cell a"     },
   { "pn"         ,  2, ycalc_printnum          , 'f', "n:a"    , 's', "gyges trimmed print string as value of cell a"     },
   { "lpad"       ,  4, ycalc_lpad              , 'f', "s:sn"   , 's', "add whitespace to start of n until x length"       },
   { "rpad"       ,  4, ycalc_rpad              , 'f', "s:sn"   , 's', "add whitespace to end of n until x length"         },
   { "lppad"      ,  5, ycalc_lppad             , 'f', "s:an"   , 's', "add whitespace to start of printable till x len"   },
   { "rppad"      ,  5, ycalc_rppad             , 'f', "s:an"   , 's', "add whitespace to end of printable till x len"     },
   { "find"       ,  4, ycalc_find              , 'f', "s:ss"   , 's', "find m within n"                                   },
   { "replace"    ,  7, ycalc_replace           , 'f', "s:sssn" , 's', "replace m with o within n, x times"                },
   { "ditto"      ,  5, ycalc_ditto             , 'f', "s:sn"   , 's', "replace m with o within n, x times"                },
   /*---(conversion functions)------------*/
   { "lower"      ,  5, ycalc_lower             , 'f', "s:s"    , 'c', "change all chars in n to lower case"               },
   { "upper"      ,  5, ycalc_upper             , 'f', "s:s"    , 'c', "change all chars in n to upper case"               },
   { "ascii"      ,  5, ycalc_char              , 'f', "s:n"    , 'c', "change x into an ascii character with that code"   },
   { "code"       ,  4, ycalc_code              , 'f', "n:s"    , 'c', "return ascii value of first char in n"             },
   { "v"          ,  1, ycalc_value             , 'f', "n:s"    , 'c', "ansi-c atof() to convert text number into value"   },
   { "value"      ,  5, ycalc_value             , 'f', "n:s"    , 'c', "ansi-c atof() to convert text number into value"   },
   { "atoi"       ,  4, ycalc_value             , 'f', "n:s"    , 'c', "ansi-c atof() to convert text number into value"   },
   { "atof"       ,  4, ycalc_value             , 'f', "n:s"    , 'c', "ansi-c atof() to convert text number into value"   },
   { "xtoi"       ,  4, ycalc_unhex             , 'f', "n:s"    , 'c', "ansi-c atof() to convert text number into value"   },
   { "itox"       ,  4, ycalc_hex               , 'f', "s:n"    , 'c', "ansi-c atof() to convert text number into value"   },
   { "salpha"     ,  6, ycalc_salpha            , 'f', "s:s"    , 'c', "change all non-alphabetic chars in n to '_'"       },
   { "salphac"    ,  7, ycalc_salphac           , 'f', "s:s"    , 'c', "remove all non-alphabetic chars in n"              },
   { "salnum"     ,  6, ycalc_salnum            , 'f', "s:s"    , 'c', "change all non-alphanumeric chars in n to '_'"     },
   { "salnumc"    ,  7, ycalc_salnumc           , 'f', "s:s"    , 'c', "remove all non-alphanumeric chars in n"            },
   { "sbasic"     ,  6, ycalc_sbasic            , 'f', "s:s"    , 'c', "change all non-basic chars in n to '_'"            },
   { "sbasicc"    ,  7, ycalc_sbasicc           , 'f', "s:s"    , 'c', "remove all non-basic chars in n"                   },
   { "swrite"     ,  6, ycalc_swrite            , 'f', "s:s"    , 'c', "change all non-writing chars in n to '_'"          },
   { "swritec"    ,  7, ycalc_swritec           , 'f', "s:s"    , 'c', "remove all non-writing chars in n"                 },
   { "sexten"     ,  6, ycalc_sexten            , 'f', "s:s"    , 'c', "change all non-extended chars in n to '_'"         },
   { "sextenc"    ,  7, ycalc_sextenc           , 'f', "s:s"    , 'c', "change all non-extended chars in n"                },
   { "sprint"     ,  6, ycalc_sprint            , 'f', "s:s"    , 'c', "change all non-printable chars in n to '_'"        },
   { "sprintc"    ,  7, ycalc_sprintc           , 'f', "s:s"    , 'c', "change all non-printable chars in n"               },
   { "sseven"     ,  6, ycalc_sseven            , 'f', "s:s"    , 'c', "change all non-7bit safe chars in n to '_'"        },
   { "ssevenc"    ,  7, ycalc_ssevenc           , 'f', "s:s"    , 'c', "remove all non-7bit safe chars in n"               },
   /*---(object audit functions)----------*/
   { "type"       ,  4, ycalc_type              , 'f', "n:a"    , 'i', "returns the type character as a value"             },
   { "isblank"    ,  7, ycalc_isblank           , 'f', "t:a"    , 'i', "T if cell a is numeric value"                      },
   { "isvalue"    ,  7, ycalc_isvalue           , 'f', "t:a"    , 'i', "T if cell a is numeric value"                      },
   { "istext"     ,  6, ycalc_istext            , 'f', "t:a"    , 'i', "T if cell a is numeric value"                      },
   { "iscalc"     ,  6, ycalc_iscalc            , 'f', "t:a"    , 'i', "T if cell a is numeric value"                      },
   { "islit"      ,  5, ycalc_islit             , 'f', "t:a"    , 'i', "T if cell a is numeric value"                      },
   { "ispoint"    ,  7, ycalc_ispoint           , 'f', "t:a"    , 'i', "T if cell a is cell or range pointer"              },
   { "iserror"    ,  7, ycalc_iserror           , 'f', "t:a"    , 'i', "T if cell a is in error status"                    },
   { "upos"       ,  4, ycalc_upos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "tab"        ,  3, ycalc_upos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "xpos"       ,  4, ycalc_xpos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "col"        ,  3, ycalc_xpos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "ypos"       ,  4, ycalc_ypos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "row"        ,  3, ycalc_ypos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "zpos"       ,  4, ycalc_zpos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "dep"        ,  3, ycalc_zpos              , 'f', "n:a"    , 'i', "T if cell a is numeric value"                      },
   { "me"         ,  2, ycalc_me                , 'f', "a:"     , 'i', "identifies the current cell for use"               },
   { "label"      ,  5, ycalc_label             , 'f', "s:a"    , 'i', "cell label of cell requested"                      },
   { "formula"    ,  7, ycalc_formula           , 'f', "s:a"    , 'i', "formula source of cell a"                          },
   { "f"          ,  1, ycalc_formula           , 'f', "s:a"    , 'i', "formula source of cell a"                          },
   { "rpn"        ,  3, ycalc_rpn               , 'f', "s:a"    , 'i', "rpn version of cell a formula"                     },
   { "ncalc"      ,  5, ycalc_ncalc             , 'f', "n:a"    , 'i', "count of formula tokens in cell a "                },
   { "nrpn"       ,  4, ycalc_ncalc             , 'f', "n:a"    , 'i', "count of formula tokens in cell a "                },
   { "reqs"       ,  4, ycalc_reqs              , 'f', "s:a"    , 'i', "list of cells required by cell a"                  },
   { "nreq"       ,  4, ycalc_nreq              , 'f', "n:a"    , 'i', "count of cells required by cell a"                 },
   { "pros"       ,  4, ycalc_pros              , 'f', "s:a"    , 'i', "list of cells provided to by cell a"               },
   { "npro"       ,  4, ycalc_npro              , 'f', "n:a"    , 'i', "count of cells provided to by cell a"              },
   { "level"      ,  5, ycalc_level             , 'f', "n:a"    , 'i', "level of seqencing"                                },
   { "*:"         ,  2, ycalc_pointer           , 'o', "a:a"    , 'i', "indirect access using pointer-like interface"      },
   /*---(trig functions)------------------*/
   { "rad"        ,  3, ycalc_radians           , 'f', "n:n"    , 't', "translate x degrees into radians"                  },
   { "Í"          ,  1, ycalc_radians           , 'f', "n:n"    , 't', "translate x degrees into radians"                  },
   { "deg"        ,  3, ycalc_degrees           , 'f', "n:n"    , 't', "translate x radians into degrees"                  },
   { "Ì"          ,  1, ycalc_degrees           , 'f', "n:n"    , 't', "translate x radians into degrees"                  },
   { "pi"         ,  2, ycalc_pi                , 'f', "n:"     , 't', "value of PI to 7 decimal places"                   },
   { "hypot"      ,  5, ycalc_hypot             , 'f', "n:nn"   , 't', "hypotenuse length given sides x and y"             },
   { "side"       ,  4, ycalc_side              , 'f', "n:nn"   , 't', "side length given hypotenuse and other side"       },
   /*-------*/
   { "ù"          ,  1, ycalc_sin               , 'f', "n:n"    , 't', "sine (degrees)"                                    },
   { "sin"        ,  3, ycalc_sin               , 'f', "n:n"    , 't', "sine (degrees)"                                    },
   { "sinr"       ,  4, ycalc_sinr              , 'f', "n:n"    , 't', "sine (radians)"                                    },
   { "ý"          ,  1, ycalc_cos               , 'f', "n:n"    , 't', "cosine (degrees)"                                  },
   { "cos"        ,  3, ycalc_cos               , 'f', "n:n"    , 't', "cosine (degrees)"                                  },
   { "cosr"       ,  4, ycalc_cosr              , 'f', "n:n"    , 't', "cosine (radians)"                                  },
   /*-------*/
   { "ú"          ,  1, ycalc_tan               , 'f', "n:n"    , 't', "tangent (degrees)"                                 },
   { "tan"        ,  3, ycalc_tan               , 'f', "n:n"    , 't', "tangent (degrees)"                                 },
   { "tanr"       ,  4, ycalc_tanr              , 'f', "n:n"    , 't', "tangent (radians)"                                 },
   { "cot"        ,  3, ycalc_cot               , 'f', "n:n"    , 't', "cotangent (degrees)"                               },
   { "cotr"       ,  4, ycalc_cotr              , 'f', "n:n"    , 't', "cotangent (radians)"                               },
   /*-------*/
   { "sec"        ,  3, ycalc_sec               , 'f', "n:n"    , 't', "secant (degrees)"                                  },
   { "secr"       ,  4, ycalc_secr              , 'f', "n:n"    , 't', "secant (radians)"                                  },
   { "csc"        ,  3, ycalc_csc               , 'f', "n:n"    , 't', "cosecant (degrees)"                                },
   { "cscr"       ,  4, ycalc_cscr              , 'f', "n:n"    , 't', "cosecant (radians)"                                },
   /*-------*/
   { "xsec"       ,  4, ycalc_xsec              , 'f', "n:n"    , 't', "external secant (degrees)"                         },
   { "xsecr"      ,  5, ycalc_xsecr             , 'f', "n:n"    , 't', "external secant (radians)"                         },
   { "xcsc"       ,  4, ycalc_xcsc              , 'f', "n:n"    , 't', "external cosecant (degrees)"                       },
   { "xcscr"      ,  5, ycalc_xcscr             , 'f', "n:n"    , 't', "external cosecant (radians)"                       },
   /*-------*/
   { "siv"        ,  3, ycalc_vsin              , 'f', "n:n"    , 't', "versed sine (degrees)"                             },
   { "sivr"       ,  4, ycalc_vsinr             , 'f', "n:n"    , 't', "versed sine (radians)"                             },
   { "vcos"       ,  4, ycalc_vcos              , 'f', "n:n"    , 't', "versed cosine (degrees)"                           },
   { "vcosr"      ,  5, ycalc_vcosr             , 'f', "n:n"    , 't', "versed cosine (radians)"                           },
   { "csin"       ,  4, ycalc_csin              , 'f', "n:n"    , 't', "coversed sine (degrees)"                           },
   { "csinr"      ,  5, ycalc_csinr             , 'f', "n:n"    , 't', "coversed sine (radians)"                           },
   { "ccos"       ,  4, ycalc_ccos              , 'f', "n:n"    , 't', "coversed cosine (degrees)"                         },
   { "ccosr"      ,  5, ycalc_ccosr             , 'f', "n:n"    , 't', "coversed cosine (radians)"                         },
   /*-------*/
   { "hsiv"       ,  4, ycalc_hvsin             , 'f', "n:n"    , 't', "half-versed sine (degrees)"                        },
   { "hsivr"      ,  5, ycalc_hvsinr            , 'f', "n:n"    , 't', "half-versed sine (radians)"                        },
   { "hvcos"      ,  5, ycalc_hvcos             , 'f', "n:n"    , 't', "half-versed cosine (degrees)"                      },
   { "hvcosr"     ,  6, ycalc_hvcosr            , 'f', "n:n"    , 't', "half-versed cosine (radians)"                      },
   { "hcsin"      ,  5, ycalc_hcsin             , 'f', "n:n"    , 't', "half-coversed sine (degrees)"                      },
   { "hcsinr"     ,  6, ycalc_hcsinr            , 'f', "n:n"    , 't', "half-coversed sine (radians)"                      },
   { "hccos"      ,  5, ycalc_hccos             , 'f', "n:n"    , 't', "half-coversed cosine (degrees)"                    },
   { "hccosr"     ,  6, ycalc_hccosr            , 'f', "n:n"    , 't', "half-coversed cosine (radians)"                    },
   /*-------*/
   { "crd"        ,  3, ycalc_crd               , 'f', "n:n"    , 't', "chord (degrees)"                                   },
   { "crdr"       ,  4, ycalc_crdr              , 'f', "n:n"    , 't', "chord (radians)"                                   },
   { "sag"        ,  3, ycalc_sag               , 'f', "n:n"    , 't', "sagitta (degrees)"                                 },
   { "sagr"       ,  4, ycalc_sagr              , 'f', "n:n"    , 't', "sagitta (radians)"                                 },
   { "apo"        ,  3, ycalc_apo               , 'f', "n:n"    , 't', "apothem (degrees)"                                 },
   { "apo"        ,  4, ycalc_apor              , 'f', "n:n"    , 't', "apothem (radians)"                                 },
   { "csag"       ,  4, ycalc_csg               , 'f', "n:n"    , 't', "long sagitta (degrees)"                            },
   { "csagr"      ,  5, ycalc_csgr              , 'f', "n:n"    , 't', "long sagitta (radians)"                            },
   { "fsag"       ,  4, ycalc_fsag              , 'f', "n:n"    , 't', "long sagitta (degrees)"                            },
   { "arc"        ,  3, ycalc_arc               , 'f', "n:n"    , 't', "arc length given angle (degrees)"                  },
   { "arcr"       ,  4, ycalc_arcr              , 'f', "n:n"    , 't', "arc length given angle (radians)"                  },
   { "los"        ,  3, ycalc_los               , 'f', "n:n"    , 't', "line of sight (height, radius)"                    },
   { "los2"       ,  4, ycalc_los2              , 'f', "n:nn"   , 't', "line of sight (height, radius)"                    },
   { "xsag"       ,  4, ycalc_xsag              , 'f', "n:n"    , 't', "watch tower height (ex-sagitta) (line of sight max, radius)"    },
   { "xsag2"      ,  5, ycalc_xsag2             , 'f', "n:nn"   , 't', "watch tower height (ex-sagitta) (line of sight max, radius)"    },
   { "hcrd"       ,  4, ycalc_hcrd              , 'f', "n:n"    , 't', "half chord (degrees)"                              },
   { "hcrdr"      ,  5, ycalc_hcrdr             , 'f', "n:n"    , 't', "half chord (radians)"                              },
   { "scrd"       ,  4, ycalc_scrd              , 'f', "n:n"    , 't', "supplemental chord (degrees)"                      },
   { "scrdr"      ,  5, ycalc_scrdr             , 'f', "n:n"    , 't', "supplemental chord (radians)"                      },
   { "vcrd"       ,  4, ycalc_vcrd              , 'f', "n:n"    , 't', "versed chord (degrees)"                            },
   { "vcrdr"      ,  5, ycalc_vcrdr             , 'f', "n:n"    , 't', "versed chord (radians)"                            },
   { "hvcrd"      ,  5, ycalc_hvcrd             , 'f', "n:n"    , 't', "half-versed chord (degrees)"                       },
   { "hvcrdr"     ,  6, ycalc_hvcrdr            , 'f', "n:n"    , 't', "half-versed chord (radians)"                       },
   /*-------*/
   { "asin"       ,  4, ycalc_asin              , 'f', "n:n"    , 't', "arcsine in degrees"                                },
   { "asinr"      ,  5, ycalc_asinr             , 'f', "n:n"    , 't', "arcsine in radians"                                },
   { "acos"       ,  4, ycalc_acos              , 'f', "n:n"    , 't', "arcosine in degrees"                               },
   { "acosr"      ,  5, ycalc_acosr             , 'f', "n:n"    , 't', "arcosine in radians"                               },
   { "atan"       ,  4, ycalc_atan              , 'f', "n:n"    , 't', "arctangent in degrees"                             },
   { "atanr"      ,  5, ycalc_atanr             , 'f', "n:n"    , 't', "arctangent in radians"                             },
   { "atan2"      ,  5, ycalc_atan2             , 'f', "n:nn"   , 't', "arctangent in degrees given sides x and y"         },
   { "atanr2"     ,  6, ycalc_atanr2            , 'f', "n:nn"   , 't', "arctangent in radians given sides x and y"         },
   { "acrd2"      ,  5, ycalc_acrd2             , 'f', "n:nn"   , 't', "arc-chord in degrees given chord len and radius"   },
   /*---(range info functions)------------*/
   { "dist"       ,  4, ycalc_dist              , 'f', "v:r"    , 'r', "geometric distance between beg and end locations"  },
   { "tabs"       ,  4, ycalc_tabs              , 'f', "v:r"    , 'r', "number of tabs in range"                           },
   { "b_size"     ,  6, ycalc_tabs              , 'f', "v:r"    , 'r', "number of tabs in range"                           },
   { "cols"       ,  4, ycalc_cols              , 'f', "v:r"    , 'r', "number of columns in range"                        },
   { "x_size"     ,  6, ycalc_cols              , 'f', "v:r"    , 'r', "number of columns in range"                        },
   { "rows"       ,  4, ycalc_rows              , 'f', "v:r"    , 'r', "number of rows in range"                           },
   { "y_size"     ,  6, ycalc_rows              , 'f', "v:r"    , 'r', "number of rows in range"                           },
   { "levels"     ,  6, ycalc_levels            , 'f', "v:r"    , 'r', "number of rows in range"                           },
   { "z_size"     ,  6, ycalc_levels            , 'f', "v:r"    , 'r', "number of rows in range"                           },
   { "sum"        ,  3, ycalc_sum               , 'f', "v:r"    , 'r', "sum of numeric cells in range"                     },

   { "every"      ,  5, ycalc_every             , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "used"       ,  4, ycalc_used              , 'f', "v:r"    , 'r', "count of filled cells in range"                    , "" },
   { "count"      ,  5, ycalc_used              , 'f', "v:r"    , 'r', "count of filled cells in range"                    , "" },
   { "empties"    ,  7, ycalc_empties           , 'f', "v:r"    , 'r', "count of every cell in range"                      },

   { "numbers"    ,  7, ycalc_numbers           , 'f', "v:r"    , 'r', "count of numeric cells in range"                   , "" },
   { "countn"     ,  6, ycalc_numbers           , 'f', "v:r"    , 'r', "count of numeric cells in range"                   , "" },
   { "nlits"      ,  5, ycalc_nlit              , 'f', "v:r"    , 'r', "count of numeric cells in range"                   , "" },
   { "nforms"     ,  6, ycalc_nform             , 'f', "v:r"    , 'r', "count of numeric cells in range"                   , "" },
   { "nlikes"     ,  6, ycalc_nlike             , 'f', "v:r"    , 'r', "count of numeric cells in range"                   , "" },

   { "strings"    ,  7, ycalc_strings           , 'f', "v:r"    , 'r', "count of string cells in range"                    },
   { "counta"     ,  6, ycalc_strings           , 'f', "v:r"    , 'r', "count of numeric cells in range"                   , "" },
   { "slits"      ,  5, ycalc_slit              , 'f', "v:r"    , 'r', "count of string cells in range"                    },
   { "sforms"     ,  6, ycalc_sform             , 'f', "v:r"    , 'r', "count of string cells in range"                    },
   { "slikes"     ,  6, ycalc_slike             , 'f', "v:r"    , 'r', "count of string cells in range"                    },

   { "points"     ,  6, ycalc_pointers          , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "merges"     ,  6, ycalc_merges            , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "blanks"     ,  6, ycalc_blanks            , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "errors"     ,  6, ycalc_errors            , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "others"     ,  6, ycalc_unknowns          , 'f', "v:r"    , 'r', "count of every cell in range"                      },

   { "lits"       ,  4, ycalc_literals          , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "forms"      ,  5, ycalc_forms             , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "likes"      ,  5, ycalc_likes             , 'f', "v:r"    , 'r', "count of every cell in range"                      },
   { "calcs"      ,  5, ycalc_calcs             , 'f', "v:r"    , 'r', "count of every cell in range"                      },

   { "avg"        ,  3, ycalc_average           , 'f', "v:r"    , 'r', "average of numeric cells in range"                 },
   { "mean"       ,  4, ycalc_average           , 'f', "v:r"    , 'r', "average of numeric cells in range"                 },
   { "min"        ,  3, ycalc_min               , 'f', "v:r"    , 'r', "minimum value in range"                            },
   { "max"        ,  3, ycalc_max               , 'f', "v:r"    , 'r', "maximum value in range"                            },
   { "range"      ,  5, ycalc_range             , 'f', "v:r"    , 'r', "difference between min and max in range"           },
   { "qtr0"       ,  4, ycalc_min               , 'f', "v:r"    , 'r', "minimum value in range"                            },
   { "qtr1"       ,  4, ycalc_quarter1          , 'f', "v:r"    , 'r', "first quarter value in range"                      },
   { "qtr2"       ,  4, ycalc_median            , 'f', "v:r"    , 'r', "second quarter (average) value in range"           },
   { "qtr3"       ,  4, ycalc_quarter3          , 'f', "v:r"    , 'r', "third quarter value in range"                      },
   { "qtr4"       ,  4, ycalc_max               , 'f', "v:r"    , 'r', "maximum value in range"                            },
   { "rangeq"     ,  6, ycalc_rangeq            , 'f', "v:r"    , 'r', "difference between q1 and q3 in range"             },
   { "median"     ,  6, ycalc_median            , 'f', "v:r"    , 'r', "median value in range"                             },
   { "mode"       ,  4, ycalc_mode              , 'f', "v:r"    , 'r', "mode of range"                                     },
   { "stddev"     ,  6, ycalc_stddev            , 'f', "v:r"    , 'r', "standard deviation in range"                       },
   { "skew"       ,  4, ycalc_skew              , 'f', "v:r"    , 'r', "quartile-based skewness measure"                   },
   /*---(address functions)---------------*/
   /*---(rel to ref)----*/
   { "ru"         ,  2, ycalc_ru                , 'f', "r:rv"   , 'a', "create a reference relative to current"            },
   { "rx"         ,  2, ycalc_rx                , 'f', "r:rv"   , 'a', "create a reference relative to current"            },
   { "ry"         ,  2, ycalc_ry                , 'f', "r:rv"   , 'a', "create a reference relative to current"            },
   { "rxy"        ,  3, ycalc_rxy               , 'f', "r:rvv"  , 'a', "create a reference relative to current"            },
   { "ruxy"       ,  4, ycalc_ruxy              , 'f', "r:rvvv" , 'a', "create a reference relative to current"            },
   /*---(abs to ref)----*/
   { "au"         ,  2, ycalc_au                , 'f', "r:rv"   , 'a', "create a reference relative to current"            },
   { "ax"         ,  2, ycalc_ax                , 'f', "r:rv"   , 'a', "create a reference relative to current"            },
   { "ay"         ,  2, ycalc_ay                , 'f', "r:rv"   , 'a', "create a reference relative to current"            },
   { "axy"        ,  3, ycalc_axy               , 'f', "r:rvv"  , 'a', "create a reference relative to current"            },
   { "auxy"       ,  4, ycalc_auxy              , 'f', "r:rvvv" , 'a', "create a reference relative to current"            },
   /*---(rel to self)---*/
   { "relu"       ,  4, ycalc_rel_b             , 'f', "r:v"    , 'a', "create a reference relative to current"            },
   { "relx"       ,  4, ycalc_rel_x             , 'f', "r:v"    , 'a', "create a reference relative to current"            },
   { "rely"       ,  4, ycalc_rel_y             , 'f', "r:v"    , 'a', "create a reference relative to current"            },
   { "relxy"      ,  5, ycalc_rel_xy            , 'f', "r:vv"   , 'a', "create a reference relative to current"            },
   { "reluxy"     ,  6, ycalc_rel_bxy           , 'f', "r:vvv"  , 'a', "create a reference relative to current"            },
   /*---(abs to self)---*/
   { "absu"       ,  4, ycalc_abs_b             , 'f', "r:v"    , 'a', "create a reference relative to current"            },
   { "absx"       ,  4, ycalc_abs_x             , 'f', "r:v"    , 'a', "create a reference relative to current"            },
   { "absy"       ,  4, ycalc_abs_y             , 'f', "r:v"    , 'a', "create a reference relative to current"            },
   { "absxy"      ,  5, ycalc_abs_xy            , 'f', "r:vv"   , 'a', "create a reference relative to current"            },
   { "absuxy"     ,  6, ycalc_abs_bxy           , 'f', "r:vvv"  , 'a', "create a reference relative to current"            },
   /*---(specific)------*/
   { "addr"       ,  4, ycalc_address           , 'f', "r:vvv"  , 'a', "create a reference relative to current"            },
   { "address"    ,  7, ycalc_address           , 'f', "r:vvv"  , 'a', "create a reference relative to current"            },
   /*---(lookup functions)----------------*/
   { "vlook"      ,  5, ycalc_vlookup           , 'f', "r:rsv"  , 'f', "contents of cell x to right of one matching n"     },
   { "vpre"       ,  4, ycalc_vprefix           , 'f', "r:rsv"  , 'f', "contents of cell x to right of one matching n"     },
   { "vmatch"     ,  6, ycalc_vmatch            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "vrange"     ,  6, ycalc_vrange            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "vabout"     ,  6, ycalc_vabout            , 'f', "r:rvvv" , 'f', "contents of cell x to right of one matching n"     },
   { "vclose"     ,  6, ycalc_vclose            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "vover"      ,  5, ycalc_vover             , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "vunder"     ,  6, ycalc_vunder            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "hlook"      ,  5, ycalc_hlookup           , 'f', "r:rsv"  , 'f', "contents of cell x below one matching n"     },
   { "hpre"       ,  4, ycalc_vprefix           , 'f', "r:rsv"  , 'f', "contents of cell x below one matching n"     },
   { "hmatch"     ,  6, ycalc_hmatch            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "hrange"     ,  6, ycalc_hrange            , 'f', "r:rvvv" , 'f', "contents of cell x to right of one matching n"     },
   { "habout"     ,  6, ycalc_habout            , 'f', "r:rvvv" , 'f', "contents of cell x to right of one matching n"     },
   { "hclose"     ,  6, ycalc_hclose            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "hover"      ,  5, ycalc_hover             , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "hunder"     ,  6, ycalc_hunder            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "rlook"      ,  5, ycalc_rlookup           , 'f', "r:rs"   , 'f', "contents of cell x below one matching n"     },
   { "rpre"       ,  4, ycalc_rprefix           , 'f', "r:rs"   , 'f', "contents of cell x below one matching n"     },
   { "rmatch"     ,  6, ycalc_rmatch            , 'f', "r:rv"   , 'f', "contents of cell x to right of one matching n"     },
   { "rrange"     ,  6, ycalc_rrange            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "rabout"     ,  6, ycalc_rabout            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "rclose"     ,  6, ycalc_rclose            , 'f', "r:rv"   , 'f', "contents of cell x to right of one matching n"     },
   { "rover"      ,  5, ycalc_rover             , 'f', "r:rv"   , 'f', "contents of cell x to right of one matching n"     },
   { "runder"     ,  6, ycalc_runder            , 'f', "r:rv"   , 'f', "contents of cell x to right of one matching n"     },
   { "ilook"      ,  5, ycalc_ilookup           , 'f', "r:rsv"  , 'f', "contents of cell x below one matching n"     },
   { "ipre"       ,  4, ycalc_iprefix           , 'f', "r:rsv"  , 'f', "contents of cell x below one matching n"     },
   { "imatch"     ,  6, ycalc_imatch            , 'f', "r:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "irange"     ,  6, ycalc_irange            , 'f', "r:rvvv" , 'f', "contents of cell x to right of one matching n"     },
   { "iabout"     ,  6, ycalc_iabout            , 'f', "r:rvvv" , 'f', "contents of cell x to right of one matching n"     },
   { "clook"      ,  5, ycalc_clookup           , 'f', "v:rs"   , 'f', "contents of cell x below one matching n"     },
   { "cpre"       ,  4, ycalc_cprefix           , 'f', "v:rs"   , 'f', "contents of cell x below one matching n"     },
   { "cmatch"     ,  6, ycalc_cmatch            , 'f', "v:rv"   , 'f', "contents of cell x to right of one matching n"     },
   { "crange"     ,  6, ycalc_crange            , 'f', "v:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "cabout"     ,  6, ycalc_cabout            , 'f', "v:rvv"  , 'f', "contents of cell x to right of one matching n"     },
   { "entry"      ,  5, ycalc_entry             , 'f', "r:v"    , 'f', "first entry next to or above current in range"     },
   /*---(date functions)------------------*/
   { "today"      ,  5, ycalc_now               , 'f', "v:"     , 'd', "current unix epoch (time number) in seconds"       },
   { "now"        ,  3, ycalc_now               , 'f', "v:"     , 'd', "current unix epoch (time number) in seconds"       },
   { "yrs"        ,  3, ycalc_year              , 'f', "v:v"    , 'd', "year number of time number"                        },
   { "mon"        ,  3, ycalc_month             , 'f', "v:v"    , 'd', "month number (0-11) of time number"                },
   { "day"        ,  3, ycalc_day               , 'f', "v:v"    , 'd', "day number (0-31) of time number"                  },
   { "hrs"        ,  3, ycalc_hour              , 'f', "v:v"    , 'd', "hour number (0-23) of time number"                 },
   { "mns"        ,  3, ycalc_minute            , 'f', "v:v"    , 'd', "minute number (0-59) of time number"               },
   { "sec"        ,  3, ycalc_second            , 'f', "v:v"    , 'd', "second number (0-59) of time number"               },
   { "dow"        ,  7, ycalc_weekday           , 'f', "v:v"    , 'd', "weekday number (0-6) of time number"               },
   { "woy"        ,  7, ycalc_weeknum           , 'f', "v:v"    , 'd', "week number (0-54) of time number"                 },
   { "doy"        ,  6, ycalc_daynum            , 'f', "v:v"    , 'd', "week number (0-54) of time number"                 },
   { "datevalue"  ,  9, ycalc_timevalue         , 'f', "v:s"    , 'd', "converts string format date to epoch number"       },
   { "dv"         ,  2, ycalc_timevalue         , 'f', "v:s"    , 'd', "converts string format date to epoch number"       },
   { "date"       ,  4, ycalc_date              , 'f', "v:vvv"  , 'd', "turns year, month, day values into eqoch number"   },
   { "time"       ,  4, ycalc_time              , 'f', "v:vvv"  , 'd', "turns hour, min, sec values into eqoch number"     },
   { "datepart"   ,  8, ycalc_datepart          , 'f', "v:v"    , 'd', "incremental epoch yy:mm:dd part of epoch number"   },
   { "timepart"   ,  8, ycalc_timepart          , 'f', "v:v"    , 'd', "incremental epoch hh:mm:ss part of epoch number"   },
   { "tzsec"      ,  5, ycalc_timezonesec       , 'f', "v:"     , 'd', "number of seconds to adjust epoch for localtime"   },
   /*---(end-of-s_funcs)--------------------*/
   { "END"        ,  0, NULL                    , '-', ""       , '-', ""                                                  },
};




/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        yCALC_ver     [500];

char*      /* ---- : return library versioning information -------------------*/
yCALC_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strlcpy (t, "[hephaestus ]", 15);
#else
   strlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (yCALC_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return yCALC_ver;
}

char
ycalc_not_ready         (void)
{
   if (myCALC.status != 'O') {
      DEBUG_YCALC   yLOG_note    ("system not operational, must init and config");
      DEBUG_YCALC   yLOG_char    ("status"    , myCALC.status);
      DEBUG_YCALC   yLOG_info    ("detail"    , myCALC.status_detail);
      return 1;
   }
   DEBUG_YCALC   yLOG_note    ("system operational, ready");
   return 0;
}

char
ycalc_status_allow      (char a_step)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_pos       =    0;
   char       *x_valid     = "ielv";
   /*---(allowed)------------------------*/
   DEBUG_YCALC   yLOG_value   ("a_step"    , a_step);
   --rce;  if (a_step == 0)  {
      DEBUG_YCALC   yLOG_note    ("must provide a step abbreviation");
      return rce;
   }
   DEBUG_YCALC   yLOG_char    ("a_step"    , a_step);
   DEBUG_YCALC   yLOG_info    ("valid"     , x_valid);
   --rce;  if (strchr (x_valid, a_step) == NULL) {
      DEBUG_YCALC   yLOG_note    ("not a valid step");
      return rce;
   }
   /*---(sequence)-----------------------*/
   --rce;  if (a_step == 'i' && myCALC.status_detail [2] == 'i') {
      DEBUG_YCALC   yLOG_note    ("already called init, can not re-initialize");
      return rce;
   }
   --rce;  if (strchr ("elv", a_step) != NULL && myCALC.status_detail [2] != 'i') {
      DEBUG_YCALC   yLOG_note    ("can not call any config before initialization");
      return rce;
   }
   --rce;  if (a_step == 'l' && myCALC.status_detail [4] != 'e') {
      DEBUG_YCALC   yLOG_note    ("can not label config before exist config");
      return rce;
   }
   --rce;  if (a_step == 'v' && myCALC.status_detail [4] != 'e') {
      DEBUG_YCALC   yLOG_note    ("can not value config before exist config");
      return rce;
   }
   --rce;  if (a_step == 'v' && myCALC.status_detail [5] != 'l') {
      DEBUG_YCALC   yLOG_note    ("can not value config before label config");
      return rce;
   }
   /*---(display)------------------------*/
   DEBUG_YCALC   yLOG_char    ("status"    , myCALC.status);
   DEBUG_YCALC   yLOG_info    ("detail"    , myCALC.status_detail);
   /*---(complete)-----------------------*/
   return 0;
}

char
ycalc_status_update     (char a_step)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_pos       =    0;
   struct tm  *temp;
   ullong      x_now       =    0;
   /*---(position)-----------------------*/
   if (a_step == 0) {
      myCALC.status = '-';
      strlcpy (myCALC.status_detail, "[ - --- - ]", LEN_LABEL);
      return 0;
   }
   /*---(position)-----------------------*/
   switch (a_step) {
   case 'i' : x_pos = 2;  myCALC.status = 'I';  break;
   case 'e' : x_pos = 4;  break;
   case 'l' : x_pos = 5;  break;
   case 'v' : x_pos = 6;  break;
   default  : return -1;
   }
   /*---(update)-------------------------*/
   myCALC.status_detail [x_pos] = a_step;
   /*---(check)--------------------------*/
   if (strncmp (myCALC.status_detail + 4, "elv", 3)  == 0) {
      myCALC.status = 'O';
      myCALC.status_detail [8] = 'o';
   }
   /*---(time)---------------------------*/
   x_now     = 0;
   temp      = gmtime (&x_now);
   time_zone = mktime (temp);
   /*---(display)------------------------*/
   DEBUG_YCALC   yLOG_char    ("status"    , myCALC.status);
   DEBUG_YCALC   yLOG_info    ("detail"    , myCALC.status_detail);
   /*---(complete)-----------------------*/
   return 0;
}

char
yCALC_init              (char a_style)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ycalc_status_allow  ('i');
   DEBUG_YCALC   yLOG_value   ("allow"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ycalc_status_update (0);
   DEBUG_YCALC   yLOG_char    ("status"    , myCALC.status);
   DEBUG_YCALC   yLOG_info    ("detail"    , myCALC.status_detail);
   /*---(functions)----------------------*/
   DEBUG_YCALC   yLOG_note    ("clearing function calls");
   if (a_style == 'g')  yRPN_init (YRPN_GYGES);
   else                 yRPN_init (YRPN_CBANG);
   /*---(functions)----------------------*/
   DEBUG_YCALC   yLOG_note    ("clearing function calls");
   myCALC.e_who_named = NULL;
   myCALC.e_who_at    = NULL;
   myCALC.e_labeler   = NULL;
   myCALC.e_enabler   = NULL;
   myCALC.e_pointer   = NULL;
   myCALC.e_reaper    = NULL;
   myCALC.e_valuer    = NULL;
   myCALC.e_addresser = NULL;
   myCALC.e_special   = NULL;
   myCALC.e_printer   = NULL;
   myCALC.state = '-';
   /*---(initialize)---------------------*/
   DEBUG_YCALC   yLOG_note    ("running sub-initializations");
   ycalc_audit_init  ();
   ycalc_range_init  ();
   ycalc_build_init  ();
   ycalc_exec_init   ();
   ycalc_deps_init   ();
   ycalc_trig_init   ();
   ycalc__seq_clear  ();
   /*---(update status)------------------*/
   ycalc_status_update ('i');
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_set_state         (char a_state)
{
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   DEBUG_YCALC   yLOG_schar   (a_state);
   myCALC.state = a_state;
   DEBUG_YCALC   yLOG_schar   (myCALC.state);
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yCALC_cleanse           (void)
{
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(shutdown)-----------------------*/
   ycalc_exec_wrap   ();
   ycalc_deps_wrap   ();
   ycalc__seq_clear  ();
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_wrap              (void)
{
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(shutdown)-----------------------*/
   ycalc_exec_wrap   ();
   ycalc_deps_wrap   ();
   ycalc__seq_clear  ();
   /*---(update status)------------------*/
   ycalc_status_update (0);
   DEBUG_YCALC   yLOG_char    ("status"    , myCALC.status);
   DEBUG_YCALC   yLOG_info    ("detail"    , myCALC.status_detail);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   configuration functions                    ----===*/
/*====================------------------------------------====================*/
static void  o___CONFIG__________o () { return; }

char
yCALC_exist_config      (void *a_enabler, void *a_pointer, void *a_reaper)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ycalc_status_allow  ('e');
   DEBUG_YCALC   yLOG_value   ("allow"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update enabler)------------------*/
   DEBUG_YCALC   yLOG_point   ("enabler"    , a_enabler);
   --rce;  if (a_enabler    == NULL) {
      DEBUG_YCALC   yLOG_error   ("enabler without this callback, references, ranges, and variables can not function");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_enabler    = a_enabler;
   /*---(update pointer)------------------*/
   DEBUG_YCALC   yLOG_point   ("pointer"    , a_pointer);
   --rce;  if (a_pointer    == NULL) {
      DEBUG_YCALC   yLOG_error   ("pointer without this callback, references, ranges, and variables can not function");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_pointer    = a_pointer;
   /*---(update reaper)---------------*/
   DEBUG_YCALC   yLOG_point   ("reaper" , a_reaper);
   --rce;  if (a_reaper == NULL) {
      DEBUG_YCALC   yLOG_warn    ("reaper without this callback, a few functions may not be allowed");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_reaper = a_reaper;
   /*---(update)-------------------------*/
   DEBUG_YCALC   yLOG_note    ("updating status");
   ycalc_status_update ('e');
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_label_config      (void *a_who_named, void *a_who_at, void *a_labeler)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_owner     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ycalc_status_allow  ('l');
   DEBUG_YCALC   yLOG_value   ("allow"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update who_named)---------------*/
   DEBUG_YCALC   yLOG_point   ("who_named" , a_who_named);
   --rce;  if (a_who_named   == NULL) {
      DEBUG_YCALC   yLOG_error   ("who_named without this callback, references cannot be resolved");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_who_named  = a_who_named;
   /*---(update whois)------------------*/
   DEBUG_YCALC   yLOG_point   ("who_at"   , a_who_at);
   --rce;  if (a_who_at    == NULL) {
      DEBUG_YCALC   yLOG_error   ("whois without this callback, range dependencies are not possible");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_who_at    = a_who_at;
   /*---(update labeler)-----------------*/
   DEBUG_YCALC   yLOG_point   ("labeler"    , a_labeler);
   --rce;  if (a_labeler    == NULL) {
      DEBUG_YCALC   yLOG_error   ("labeler without this callback, human-readable verification is not possible");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_labeler    = a_labeler;
   /*---(set the dep root)---------------*/
   rc = ycalc_call_who_named ("ROOT", YCALC_FULL, &myCALC.mroot, &myCALC.rroot);
   DEBUG_YCALC   yLOG_value   ("make root" , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("mroot"     , myCALC.mroot);
   --rce;  if (myCALC.mroot == NULL) {
      DEBUG_YCALC   yLOG_note    ("must create a owner named ¸ROOT¸ for yCALC to operate");
      DEBUG_YCALC   yLOG_error   ("rroot without this, all dependencies fail");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("rroot"     , myCALC.rroot);
   --rce;  if (myCALC.rroot == NULL) {
      DEBUG_YCALC   yLOG_note    ("must create a owner named ¸ROOT¸ for yCALC to operate");
      DEBUG_YCALC   yLOG_error   ("rroot without this, all dependencies fail");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   DEBUG_YCALC   yLOG_note    ("updating status");
   ycalc_status_update ('l');
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_value_config      (void *a_valuer, void *a_addresser, void *a_special, void *a_printer)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ycalc_status_allow  ('v');
   DEBUG_YCALC   yLOG_value   ("allow"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update valuer)------------------*/
   DEBUG_YCALC   yLOG_point   ("valuer"    , a_valuer);
   --rce;  if (a_valuer    == NULL) {
      DEBUG_YCALC   yLOG_error   ("valuer without this callback, references, ranges, and variables can not function");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_valuer    = a_valuer;
   /*---(update addresser)---------------*/
   DEBUG_YCALC   yLOG_point   ("addresser" , a_addresser);
   --rce;  if (a_addresser == NULL) {
      DEBUG_YCALC   yLOG_warn    ("addresser without this callback, a few functions may not be allowed");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_addresser = a_addresser;
   /*---(update special)---------------*/
   DEBUG_YCALC   yLOG_point   ("special" , a_special);
   --rce;  if (a_special == NULL) {
      DEBUG_YCALC   yLOG_warn    ("special without this callback, a few functions may not be allowed");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_special = a_special;
   /*---(update printer)-----------------*/
   DEBUG_YCALC   yLOG_point   ("printer"   , a_printer);
   --rce;  if (a_printer   == NULL) {
      DEBUG_YCALC   yLOG_error   ("valuer without this callback, display can not be updated");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   myCALC.e_printer   = a_printer;
   /*---(update)-------------------------*/
   DEBUG_YCALC   yLOG_note    ("updating status");
   ycalc_status_update ('v');
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      special operators                       ----===*/
/*====================------------------------------------====================*/
static void  o___SPECIAL_________o () { return; }

void    /*-> tbd --------------------------------[ leaf   [fv.210.000.00]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_noop        (void)
{
   return;
}

char
ycalc_trouble_clear     (void)
{
   myCALC.trouble = YCALC_ERROR_NONE;
   return 0;
}




/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
ycalc__unit_base        (char *a_question, int a_num)
{
   char        x_error     = '-';
   /*---(initialize)---------------------*/
   strlcpy (ycalc__unit_answer, "yCALC_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if      (strncmp(a_question, "error"     , 20)  == 0) {
      x_error = myCALC.trouble;
      if (x_error == 0)  x_error = '-';
      snprintf (ycalc__unit_answer, LEN_FULL, "BASE error       : %c", x_error);
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}

char       /*----: set up program urgents/debugging --------------------------*/
ycalc__unit_quiet       (void)
{
   myCALC.logger = yLOGS_begin ("yCALC" , YLOG_SYS, YLOG_QUIET);
   myCALC.trouble = YCALC_ERROR_NONE;
   yCALC_init ('g');
   ycalc__mock_prepare ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ycalc__unit_loud        (void)
{
   char        x_argc      = 1;
   char       *x_args [1]  = { "yCALC_debug" };
   yURG_logger (x_argc, x_args);
   yURG_urgs   (x_argc, x_args);
   yURG_name   ("kitchen", YURG_ON);
   yURG_name   ("apis"   , YURG_ON);
   yURG_name   ("ystr"   , YURG_ON);
   yURG_name   ("yrpn"   , YURG_ON);
   yURG_name   ("ycalc"  , YURG_ON);
   DEBUG_YCALC   yLOG_info     ("yCALC"    , yCALC_version   ());
   myCALC.trouble = YCALC_ERROR_NONE;
   yCALC_init ('g');
   ycalc__mock_prepare ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ycalc__unit_end         (void)
{
   yCALC_wrap   ();
   ycalc__mock_cleanup ();
   ycalc_range_wrap ();
   yLOGS_end    ();
   return 0;
}



/*===[[ END ]]================================================================*/
