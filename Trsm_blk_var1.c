#include "FLAME.h"

int Trsm_blk_var1( FLA_Obj L, FLA_Obj B, int nb_alg )
{
  FLA_Obj LTL,   LTR,      L00, L01, L02, 
          LBL,   LBR,      L10, L11, L12,
                           L20, L21, L22;

  FLA_Obj BT,              B0,
          BB,              B1,
                           B2;

  int b;

  FLA_Part_2x2( L,    &LTL, &LTR,
                      &LBL, &LBR,     0, 0, FLA_TL );

  FLA_Part_2x1( B,    &BT, 
                      &BB,            0, FLA_TOP );

  while ( FLA_Obj_length( LTL ) < FLA_Obj_length( L ) ){

    b = min( FLA_Obj_length( LBR ), nb_alg );

    FLA_Repart_2x2_to_3x3( LTL, /**/ LTR,       &L00, /**/ &L01, &L02,
                        /* ************* */   /* ******************** */
                                                &L10, /**/ &L11, &L12,
                           LBL, /**/ LBR,       &L20, /**/ &L21, &L22,
                           b, b, FLA_BR );

    FLA_Repart_2x1_to_3x1( BT,                &B0, 
                        /* ** */            /* ** */
                                              &B1, 
                           BB,                &B2,        b, FLA_BOTTOM );

    /*------------------------------------------------------------*/

    /* B1 = B1 - L10 * B0 */
    FLA_Gemm( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, 
	      FLA_MINUS_ONE, L10, B0, FLA_ONE, B1 );

    /* B1 = inv( L11 ) B1 */
    Trsm_unb_var1( L11, B1 );
    //Alternatively, try
    //    FLA_Trsm( FLA_LEFT, FLA_LOWER_TRIANGULAR, FLA_NO_TRANSPOSE, FLA_NONUNIT_DIAG,
    //	      FLA_ONE, L11, B1 );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &LTL, /**/ &LTR,       L00, L01, /**/ L02,
                                                     L10, L11, /**/ L12,
                            /* ************** */  /* ****************** */
                              &LBL, /**/ &LBR,       L20, L21, /**/ L22,
                              FLA_TL );

    FLA_Cont_with_3x1_to_2x1( &BT,                B0, 
                                                  B1, 
                            /* ** */           /* ** */
                              &BB,                B2,     FLA_TOP );

  }

  return FLA_SUCCESS;
}



