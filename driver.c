#include <stdio.h>
#include <math.h>
#include <time.h>

#include "FLAME.h"

#define TEST_UNB_VAR1 FALSE
#define TEST_BLK_VAR1 FALSE
#define TEST_UNB_VAR2 FALSE
#define TEST_BLK_VAR2 FALSE
#define TEST_UNB_VAR3 FALSE
#define TEST_BLK_VAR3 FALSE
#define TEST_UNB_VAR4 FALSE
#define TEST_BLK_VAR4 FALSE


int main(int argc, char *argv[])
{
  int m, n, k, nfirst, nlast, ninc, i, irep,
    nrepeats, nb_alg, check;;

  double
    dtime,
    dtime_best,
    gflops,
    max_gflops,
    diff,
    d_n;

  FLA_Obj
    L, B, C, Cref, Cold, Bref, Bold, delta;
  
  /* Initialize FLAME */
  FLA_Init( );

  /* Every time trial is repeated "repeat" times */
  printf( "%% number of repeats:" );
  scanf( "%d", &nrepeats );
  printf( "%% %d\n", nrepeats );

  /* Enter the max GFLOPS attainable */
  printf( "%% enter max GFLOPS:" );
  scanf( "%lf", &max_gflops );
  printf( "%% %lf\n", max_gflops );

  /* Enter the algorithmic block size */
  printf( "%% enter nb_alg:" );
  scanf( "%d", &nb_alg );
  printf( "%% %d\n", nb_alg );

  /* Timing trials for matrix sizes n=nfirst to nlast in increments 
     of ninc will be performed */
  printf( "%% enter nfirst, nlast, ninc:" );
  scanf( "%d%d%d", &nfirst, &nlast, &ninc );
  printf( "%% %d %d %d\n", nfirst, nlast, ninc );

  i = 1;
  for ( n=nfirst; n<= nlast; n+=ninc ){
   
    /* Allocate space for the matrices */

    FLA_Obj_create( FLA_DOUBLE, n, n, 1, n, &L );
    FLA_Obj_create( FLA_DOUBLE, n, n, 1, n, &B );

    FLA_Obj_create( FLA_DOUBLE, n, n, 1, n, &C );
    FLA_Obj_create( FLA_DOUBLE, n, n, 1, n, &Cref );
    FLA_Obj_create( FLA_DOUBLE, n, n, 1, n, &Cold );

    FLA_Obj_create( FLA_DOUBLE, n, n, 1, n, &Bref );
    FLA_Obj_create( FLA_DOUBLE, n, n, 1, n, &Bold );

    /* Generate random matrices L and B */
    FLA_Random_matrix( L );
    FLA_Random_matrix( C );
    FLA_Random_matrix( B );
    /* Add something large to the diagonal to make sure it isn't nearly singular */
    d_n = ( double ) n;
    *( ( double * ) FLA_Obj_buffer_at_view( delta ) ) = d_n;

    FLA_Shift_diag( FLA_NO_CONJUGATE, delta, L );

    FLA_Random_matrix( Cold );

    gflops = 1.0 * n * n * n * 1.0e-09;

    /* Time FLA_Trsm */

    for ( irep=0; irep<nrepeats; irep++ ){
      FLA_Copy( Bold, Bref );

      dtime = FLA_Clock();

      FLA_Trsm( FLA_LEFT, FLA_LOWER_TRIANGULAR, FLA_NO_TRANSPOSE, FLA_NONUNIT_DIAG,
		FLA_ONE, L, Bref );

      dtime = FLA_Clock() - dtime;

      if ( irep == 0 ) 
	dtime_best = dtime;
      else
	dtime_best = ( dtime < dtime_best ? dtime : dtime_best );
    }

    printf( "data_FLAME( %d, 1:2 ) = [ %d %le ];\n", i, n,
            gflops / dtime_best );
    fflush( stdout );


    /* Time the your implementations */

#if TEST_UNB_VAR1==TRUE
    /* Variant 1 unblocked */

    for ( irep=0; irep<nrepeats; irep++ ){

      FLA_Copy( Cold, C );
    
      dtime = FLA_Clock();

      SLAP_Trmm_llt_unb_var1( L, B, C );

      dtime = FLA_Clock() - dtime;

      if ( irep == 0 ) 
	dtime_best = dtime;
      else
	dtime_best = ( dtime < dtime_best ? dtime : dtime_best );
    }    

    diff = FLA_Max_elemwise_diff( B, Bref );

    printf( "data_unb_var1( %d, 1:3 ) = [ %d %le  %le];\n", i, n,
            gflops / dtime_best, diff );
    fflush( stdout );
#endif

    FLA_Obj_free( &L );
    FLA_Obj_free( &B );
    FLA_Obj_free( &Bref );
    FLA_Obj_free( &delta );
 
    FLA_Obj_free( &C );
    FLA_Obj_free( &Cold);
    FLA_Obj_free( &Cref);
    printf( "\n" );

    i++;
  }

  /* Print the MATLAB commands to plot the data */

  /* Delete all existing figures */
  printf( "close all\n" );

  /* Plot the performance of FLAME */
  printf( "plot( data_FLAME( :,1 ), data_FLAME( :, 2 ), 'k--' ); \n" );

  /* Indicate that you want to add to the existing plot */
  printf( "hold on\n" );

  /* Plot the performance of the reference implementation */
  //  printf( "plot( data_REF( :,1 ), data_REF( :, 2 ), 'k-' ); \n" );

  /* Plot the performance of your implementations */

#if TEST_UNB_VAR1==TRUE
  printf( "plot( data_unb_var1( :,1 ), data_unb_var1( :, 2 ), 'r-.' ); \n" );
#endif
#if TEST_UNB_VAR2==TRUE
  printf( "plot( data_unb_var2( :,1 ), data_unb_var2( :, 2 ), 'g-.' ); \n" );
#endif
#if TEST_UNB_VAR3==TRUE
  printf( "plot( data_unb_var3( :,1 ), data_unb_var3( :, 2 ), 'b-.' ); \n" );
#endif
#if TEST_UNB_VAR4==TRUE
  printf( "plot( data_unb_var4( :,1 ), data_unb_var4( :, 2 ), 'm-.' ); \n" );
#endif
#if TEST_BLK_VAR1==TRUE
  printf( "plot( data_blk_var1( :,1 ), data_blk_var1( :, 2 ), 'r--' ); \n" );
#endif
#if TEST_BLK_VAR2==TRUE
  printf( "plot( data_blk_var2( :,1 ), data_blk_var2( :, 2 ), 'g--' ); \n" );
#endif
#if TEST_BLK_VAR3==TRUE
  printf( "plot( data_blk_var3( :,1 ), data_blk_var3( :, 2 ), 'b--' ); \n" );
#endif
#if TEST_BLK_VAR4==TRUE
  printf( "plot( data_blk_var4( :,1 ), data_blk_var4( :, 2 ), 'm--' ); \n" );
#endif

  printf( "hold on \n");

  printf( "xlabel( 'matrix dimension m=n' );\n");
  printf( "ylabel( 'GFLOPS/sec.' );\n");
  //  printf( "axis( [ 0 %d 0 %3.1f ] ); \n", nlast, max_gflops );
  printf( "legend( 'FLA Trsm', ...\n");
#if TEST_UNB_VAR1==TRUE
  printf( "        'unb var1', ...\n");
#endif
#if TEST_UNB_VAR2==TRUE
  printf( "        'unb var2', ...\n");
#endif
#if TEST_UNB_VAR3==TRUE
  printf( "        'unb var3', ...\n");
#endif
#if TEST_UNB_VAR4==TRUE
  printf( "        'unb var4', ...\n");
#endif
#if TEST_BLK_VAR1==TRUE
  printf( "        'blk var1', ...\n");
#endif
#if TEST_BLK_VAR2==TRUE
  printf( "        'blk var2', ...\n");
#endif
#if TEST_BLK_VAR3==TRUE
  printf( "        'blk var3', ...\n");
#endif
#if TEST_BLK_VAR4==TRUE
  printf( "        'blk var4', ...\n");
#endif
  printf( "         2 );\n");

  FLA_Finalize( );
}
