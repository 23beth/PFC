#include "FLAME.h"

int Trsm_unb_var1( FLA_Obj L, FLA_Obj B )
{
  FLA_Obj LTL,   LTR,      L00,  l01,      L02, 
          LBL,   LBR,      l10t, lambda11, l12t,
                           L20,  l21,      L22;

  FLA_Obj BT,              B0,
          BB,              b1t,
                           B2;

  FLA_Part_2x2( L,    &LTL, &LTR,
                      &LBL, &LBR,     0, 0, FLA_TL );

  FLA_Part_2x1( B,    &BT, 
                      &BB,            0, FLA_TOP );

  while ( FLA_Obj_length( LTL ) < FLA_Obj_length( L ) ){

    FLA_Repart_2x2_to_3x3( LTL, /**/ LTR,       &L00,  /**/ &l01,      &L02,
                        /* ************* */   /* *************************** */
                                                &l10t, /**/ &lambda11, &l12t,
                           LBL, /**/ LBR,       &L20,  /**/ &l21,      &L22,
                           1, 1, FLA_BR );

    FLA_Repart_2x1_to_3x1( BT,                &B0, 
                        /* ** */            /* *** */
                                              &b1t, 
                           BB,                &B2,        1, FLA_BOTTOM );

    /*------------------------------------------------------------*/

    /* b1t = b1t - l10t * B0 */
    FLA_Gemv( FLA_TRANSPOSE, FLA_MINUS_ONE, B0, l10t, FLA_ONE, b1t );

    /* b1t = b1t / lambda11 */
    FLA_Inv_scal( lambda11, b1t );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &LTL, /**/ &LTR,       L00,  l01,      /**/ L02,
                                                     l10t, lambda11, /**/ l12t,
                            /* ************** */  /* ************************* */
                              &LBL, /**/ &LBR,       L20,  l21,      /**/ L22,
                              FLA_TL );

    FLA_Cont_with_3x1_to_2x1( &BT,                B0, 
                                                  b1t, 
                            /* ** */           /* *** */
                              &BB,                B2,     FLA_TOP );

  }

  return FLA_SUCCESS;
}
