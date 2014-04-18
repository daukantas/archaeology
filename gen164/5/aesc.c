/* j/5/aes.c
**
** This file is in the public domain.
*/
#include "all.h"
#include "../pit.h"

#include <openssl/aes.h>

/* declarations
*/
  extern u2_ho_jet j2_mcj(Pt5, aesc, be)[];
  extern u2_ho_jet j2_mcj(Pt5, aesc, en)[];
  extern u2_ho_jet j2_mcj(Pt5, aesc, ex)[];

/* functions
*/
  u2_weak
  j2_mcd(Pt5, aesc, be)(u2_wire wir_r,
                        u2_bean a,
                        u2_atom b,
                        u2_atom c)
  {
    AES_KEY key_u;
    c3_y    c_y[AES_BLOCK_SIZE];

    key_u.rounds = 14;
    c3_assert(u2_cr_met(5, b) <= 4 * (AES_MAXNR + 1));
    {
      c3_w i_w;
      c3_w b_w;

      for ( i_w = 0; i_w < 4 * (AES_MAXNR + 1); i_w++ ) {
        b_w = u2_cr_word(i_w, b);
        //  sigh
        key_u.rd_key[i_w] = c3_flip32(b_w);
      }
    }

    c3_assert(u2_cr_met(3, c) <= AES_BLOCK_SIZE);
    u2_cr_bytes(0, AES_BLOCK_SIZE, c_y, c);

    if ( u2_yes == a ) {
      AES_encrypt(c_y, c_y, &key_u);
    }
    else {
      //  double sigh
      {
        c3_w i_w, j_w, tmp_w;

#       define rk key_u.rd_key
        for ( i_w = 0, j_w = 4 * key_u.rounds; i_w < j_w; i_w += 4, j_w -= 4 ) {
          tmp_w = rk[i_w    ]; rk[i_w    ] = rk[j_w    ]; rk[j_w    ] = tmp_w;
          tmp_w = rk[i_w + 1]; rk[i_w + 1] = rk[j_w + 1]; rk[j_w + 1] = tmp_w;
          tmp_w = rk[i_w + 2]; rk[i_w + 2] = rk[j_w + 2]; rk[j_w + 2] = tmp_w;
          tmp_w = rk[i_w + 3]; rk[i_w + 3] = rk[j_w + 3]; rk[j_w + 3] = tmp_w;
        }
#       undef rk
      }
      AES_decrypt(c_y, c_y, &key_u);
    }
    return u2_ci_bytes(AES_BLOCK_SIZE, c_y);
  }

  u2_weak
  j2_mc(Pt5, aesc, be)(u2_wire wir_r,
                       u2_noun cor)
  {
    u2_noun a, b, c;

    if ( u2_no == u2_mean(cor, u2_cv_sam_2, &a,
                               u2_cv_sam_6, &b,
                               u2_cv_sam_7, &c, 0) ||
         (a != u2_yes && a != u2_no) )
    {
      return u2_bl_bail(wir_r, c3__exit);
    }
    else {
      return j2_mcd(Pt5, aesc, be)(wir_r, a, b, c);
    }
  }

  u2_weak
  j2_mcd(Pt5, aesc, en)(u2_wire wir_r,
                        u2_atom a,
                        u2_atom b)
  {
    AES_KEY key_u;
    c3_y    buf_y[32];

    c3_assert(u2_cr_met(3, a) <= 32);
    c3_assert(u2_cr_met(3, b) <= 16);

    u2_cr_bytes(0, 32, buf_y, a);
    if ( 0 != AES_set_encrypt_key(buf_y, 256, &key_u) ) {
      return u2_bl_bail(wir_r, c3__exit);
    }
    else {
      u2_cr_bytes(0, 16, buf_y, b);
      AES_encrypt(buf_y, buf_y, &key_u);
    }

    return u2_ci_bytes(16, buf_y);
  }

  u2_weak
  j2_mc(Pt5, aesc, en)(u2_wire wir_r,
                       u2_noun cor)
  {
    u2_noun a, b;

    if ( u2_no == u2_mean(cor, u2_cv_sam_2, &a, u2_cv_sam_3, &b, 0) ) {
      return u2_bl_bail(wir_r, c3__exit);
    }
    else {
      return j2_mcd(Pt5, aesc, en)(wir_r, a, b);
    }
  }

  u2_weak
  j2_mcd(Pt5, aesc, ex)(u2_wire wir_r,
                        u2_atom a)
  {
    AES_KEY key_u;
    c3_y    buf_y[32];
    c3_w    out_w[4 * (AES_MAXNR + 1)];

    c3_assert(u2_cr_met(3, a) <= 32);

    u2_cr_bytes(0, 32, buf_y, a);
    if ( 0 != AES_set_encrypt_key(buf_y, 256, &key_u) ) {
      return u2_bl_bail(wir_r, c3__exit);
    }
    else {
      c3_w b_w;
      c3_w i_w;

      c3_assert(key_u.rounds == AES_MAXNR);
      for ( i_w = 0; i_w < 4 * (AES_MAXNR + 1); i_w++ ) {
        b_w = key_u.rd_key[i_w];
        //  hurk
        out_w[i_w] = c3_flip32(b_w);
      }
      return u2_ci_words(4 * (AES_MAXNR + 1), out_w);
    }
  }

  u2_weak
  j2_mc(Pt5, aesc, ex)(u2_wire wir_r,
                       u2_noun cor)
  {
    u2_noun a;

    if ( u2_no == u2_mean(cor, u2_cv_sam, &a, 0) ||
         u2_no == u2_stud(a) )
    {
      return u2_bl_bail(wir_r, c3__exit);
    }
    else {
      return j2_mcd(Pt5, aesc, ex)(wir_r, a);
    }
  }

/* structures
*/
  u2_ho_jet
  j2_mcj(Pt5, aesc, be)[] = {
    { ".2", c3__lite, j2_mc(Pt5, aesc, be), u2_jet_test | u2_jet_live, u2_none, u2_none },
    { }
  };
  u2_ho_jet
  j2_mcj(Pt5, aesc, en)[] = {
    { ".2", c3__lite, j2_mc(Pt5, aesc, en), u2_jet_test | u2_jet_live, u2_none, u2_none },
    { }
  };
  u2_ho_jet
  j2_mcj(Pt5, aesc, ex)[] = {
    { ".2", c3__lite, j2_mc(Pt5, aesc, ex), u2_jet_test | u2_jet_live, u2_none, u2_none },
    { }
  };

  u2_ho_driver
  j2_mbd(Pt5, aesc)[] = {
    { j2_sc(Pt5, aesc, be), j2_mcj(Pt5, aesc, be), 0, 0, u2_none },
    { j2_sc(Pt5, aesc, en), j2_mcj(Pt5, aesc, en), 0, 0, u2_none },
    { j2_sc(Pt5, aesc, ex), j2_mcj(Pt5, aesc, ex), 0, 0, u2_none },
    { }
  };

  u2_ho_driver
  j2_db(Pt5, aesc) =
    { j2_sb(Pt5, aesc), 0, j2_mbd(Pt5, aesc), 0, u2_none };