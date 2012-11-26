#include "FLAME.h"

int Trsm_blk_var4( FLA_Obj L, FLA_Obj B, int nb_alg )
{
  FLA_Obj BL,    BR,       B0,  B1,  B2;

  int b;

  FLA_Part_1x2( B,    &BL,  &BR,      0, FLA_RIGHT );

  while ( FLA_Obj_width( BR ) < FLA_Obj_width( B ) ){

    b = min( FLA_Obj_width( BL ), nb_alg );

    FLA_Repart_1x2_to_1x3( BL,  /**/ BR,        &B0, &B1, /**/ &B2,
                           b, FLA_LEFT );

    /*------------------------------------------------------------*/

    Trsm_unb_var4( L, B1 );

    //Alternatively, try
    //    FLA_Trsm( FLA_LEFT, FLA_LOWER_TRIANGULAR, FLA_NO_TRANSPOSE, FLA_NONUNIT_DIAG,
    //	      L, B1 );

    /*------------------------------------------------------------*/

    FLA_Cont_with_1x3_to_1x2( &BL,  /**/ &BR,        B0, /**/ B1, B2,
                              FLA_RIGHT );

  }

  return FLA_SUCCESS;
}
