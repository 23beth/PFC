
/* Copyright 2003 The University of Texas at Austin  
 
   For licensing information see
                  http://www.cs.utexas.edu/users/flame/license.html 

   Programmed by: Name of author
                  Email of author
                                                                     */

#include "FLAME.h"

int SLAP_Trmm_llt_var3( FLA_Obj L, FLA_Obj B, FLA_Obj C )
{
  FLA_Obj LTL,   LTR,      L00,  l01,      L02, 
          LBL,   LBR,      l10t, lambda11, l12t,
                           L20,  l21,      L22;

  FLA_Obj BT,              B0,
          BB,              b1t,
                           B2;

  FLA_Obj CT,              C0,
          CB,              c1t,
                           C2;

  FLA_Part_2x2( L,    &LTL, &LTR,
                      &LBL, &LBR,     0, 0, FLA_BR );

  FLA_Part_2x1( B,    &BT, 
                      &BB,            0, FLA_BOTTOM );

  FLA_Part_2x1( C,    &CT, 
                      &CB,            0, FLA_BOTTOM );

  while ( FLA_Obj_length( LBR ) < FLA_Obj_length( L ) ){

    FLA_Repart_2x2_to_3x3( LTL, /**/ LTR,       &L00,  &l01,      /**/ &L02,
                                                &l10t, &lambda11, /**/ &l12t,
                        /* ************* */   /* *************************** */
                           LBL, /**/ LBR,       &L20,  &l21,      /**/ &L22,
                           1, 1, FLA_TL );

    FLA_Repart_2x1_to_3x1( BT,                &B0, 
                                              &b1t, 
                        /* ** */            /* *** */
                           BB,                &B2,        1, FLA_TOP );

    FLA_Repart_2x1_to_3x1( CT,                &C0, 
                                              &c1t, 
                        /* ** */            /* *** */
                           CB,                &C2,        1, FLA_TOP );

    /*******************Update*********************/
	/* c1t = lambda11 * b1t + l21' * B2 + c1t*/
	c1t = FLA_Axpy(lambda11, b1t, FLA_Gemv(FLA_TRANSPOSE, ONE, B2, l21, FLA_ONE, c1t)); 
	

    FLA_Cont_with_3x3_to_2x2( &LTL, /**/ &LTR,       L00,  /**/ l01,      L02,
                            /* ************** */  /* ************************* */
                                                     l10t, /**/ lambda11, l12t,
                              &LBL, /**/ &LBR,       L20,  /**/ l21,      L22,
                              FLA_BR );

    FLA_Cont_with_3x1_to_2x1( &BT,                B0, 
                            /* ** */           /* *** */
                                                  b1t, 
                              &BB,                B2,     FLA_BOTTOM );

    FLA_Cont_with_3x1_to_2x1( &CT,                C0, 
                            /* ** */           /* *** */
                                                  c1t, 
                              &CB,                C2,     FLA_BOTTOM );

  }

  return FLA_SUCCESS;
}

