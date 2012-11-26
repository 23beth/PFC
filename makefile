#
# sample makefile
#

HOME       := /u/rvdg

# indicate where the source files are
SRC_PATH   := .

# indicate where the object files are to be created
OBJ_PATH   := .

# indicate where the FLAME libraries reside
LIB_PATH   := $(HOME)/flame/lib

# indicate where the FLAME include files reside
INC_PATH   := $(HOME)/flame/include

# indicate the path to the BLAS libraries
BLAS_PATH  := $(HOME)/flame/OpenBLAS/

# indicate what BLAS library to use
BLAS_LIB   := $(BLAS_PATH)/libopenblas_nehalem-r0.1.1.a -lpthread

# indicate the FLAME library
FLAME_BASE := $(LIB_PATH)/libflame.a

# indicate the FLAME lapack library
FLAME_LAPACK := $(HOME)/flame/lapack-3.3.0/lapack_LINUX.a

CC         := gcc
FC         := gfortran
LINKER     := $(FC)
CFLAGS     := -I$(SRC_PATH) \
              -I$(INC_PATH) \
              -g -O3 -fomit-frame-pointer -funroll-all-loops \
              -finline-functions
FFLAGS     := $(CFLAGS)
LDFLAGS    := -lm

TEST_OBJS  := driver.o \
	Trsm_unb_var1.o Trsm_blk_var1.o \
	Trsm_unb_var2.o Trsm_blk_var2.o \
	Trsm_unb_var3.o Trsm_blk_var3.o \
	Trsm_unb_var4.o Trsm_blk_var4.o \
    SLAP_Trmm_llt_var1.o \
	SLAP_Trmm_llt_var2.o

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_PATH)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

driver.x: $(TEST_OBJS)
	$(LINKER) $(TEST_OBJS) $(LDFLAGS) \
        $(FLAME_LAPACK) $(FLAME_BASE) $(BLAS_LIB) -o $(TEST_BIN) $@

# Input expected by driver.x:
# echo "nrepeats max_gflops nb_alg check nfirst nlast ninc" | ./driver.x
# nrepeats:      Repeat performance experiment nrepeats times and report fastest time
# max_gflops:    Peak performance, in GFLOPS, of processor
# nb_alg:        Algorithmic block size
# check:         Turn parameter checking ON/OFF
# nfirst, nlast, ninc:
#                Time problem sizes nfirst:ninc:nlast

# execute on a machine with peak of 6.8 GFLOPS with block size 128
# and parameter checking turned ON

run: driver.x
	echo "2 6.8 128 100 1000 100" | ./driver.x > output.m

clean:
	rm -f *.o *~ core *.x

