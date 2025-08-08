/*                                                                                
 ****************************************************************************     
 *                                                                                
 *                   "DHRYSTONE" Benchmark Program                                
 *                   -----------------------------                                
 *                                                                                
 *  Version:    C, Version 2.1                                                    
 *                                                                                
 *  File:       dhry_1.c (part 2 of 3)                                            
 *                                                                                
 *  Date:       May 25, 1988                                                      
 *                                                                                
 *  Author:     Reinhold P. Weicker                                               
 *                                                                                
 ****************************************************************************     
 */                            
 
#include "dhry.h"                                                                              
//#include <malloc.h>
#include <string.h>
#include "ppctimer.h"


/////////////////////////////////////////////////////

#ifndef REG
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#endif

extern  int     Int_Glob;
extern  char    Ch_1_Glob;


Proc_6 (Enum_Val_Par, Enum_Ref_Par)
/*********************************/
    /* executed once */
    /* Enum_Val_Par == Ident_3, Enum_Ref_Par becomes Ident_2 */

Enumeration  Enum_Val_Par;
Enumeration *Enum_Ref_Par;
{
  *Enum_Ref_Par = Enum_Val_Par;
  if (! Func_3 (Enum_Val_Par))
    /* then, not executed */
    *Enum_Ref_Par = Ident_4;
  switch (Enum_Val_Par)
  {
    case Ident_1:
      *Enum_Ref_Par = Ident_1;
      break;
    case Ident_2:
      if (Int_Glob > 100)
        /* then */
      *Enum_Ref_Par = Ident_1;
      else *Enum_Ref_Par = Ident_4;
      break;
    case Ident_3: /* executed */
      *Enum_Ref_Par = Ident_2;
      break;
    case Ident_4: break;
    case Ident_5:
      *Enum_Ref_Par = Ident_3;
      break;
  } /* switch */
} /* Proc_6 */


Proc_7 (Int_1_Par_Val, Int_2_Par_Val, Int_Par_Ref)
/**********************************************/
    /* executed three times                                      */
    /* first call:      Int_1_Par_Val == 2, Int_2_Par_Val == 3,  */
    /*                  Int_Par_Ref becomes 7                    */
    /* second call:     Int_1_Par_Val == 10, Int_2_Par_Val == 5, */
    /*                  Int_Par_Ref becomes 17                   */
    /* third call:      Int_1_Par_Val == 6, Int_2_Par_Val == 10, */
    /*                  Int_Par_Ref becomes 18                   */
One_Fifty       Int_1_Par_Val;
One_Fifty       Int_2_Par_Val;
One_Fifty      *Int_Par_Ref;
{
  One_Fifty Int_Loc;

  Int_Loc = Int_1_Par_Val + 2;
  *Int_Par_Ref = Int_2_Par_Val + Int_Loc;
} /* Proc_7 */


Proc_8 (Arr_1_Par_Ref, Arr_2_Par_Ref, Int_1_Par_Val, Int_2_Par_Val)
/*********************************************************************/
    /* executed once      */
    /* Int_Par_Val_1 == 3 */
    /* Int_Par_Val_2 == 7 */
Arr_1_Dim       Arr_1_Par_Ref;
Arr_2_Dim       Arr_2_Par_Ref;
int             Int_1_Par_Val;
int             Int_2_Par_Val;
{
  REG One_Fifty Int_Index;
  REG One_Fifty Int_Loc;

  Int_Loc = Int_1_Par_Val + 5;
  Arr_1_Par_Ref [Int_Loc] = Int_2_Par_Val;
  Arr_1_Par_Ref [Int_Loc+1] = Arr_1_Par_Ref [Int_Loc];
  Arr_1_Par_Ref [Int_Loc+30] = Int_Loc;
  for (Int_Index = Int_Loc; Int_Index <= Int_Loc+1; ++Int_Index)
    Arr_2_Par_Ref [Int_Loc] [Int_Index] = Int_Loc;
  Arr_2_Par_Ref [Int_Loc] [Int_Loc-1] += 1;
  Arr_2_Par_Ref [Int_Loc+20] [Int_Loc] = Arr_1_Par_Ref [Int_Loc];
  Int_Glob = 5;
} /* Proc_8 */


Enumeration Func_1 (Ch_1_Par_Val, Ch_2_Par_Val)
/*************************************************/
    /* executed three times                                         */
    /* first call:      Ch_1_Par_Val == 'H', Ch_2_Par_Val == 'R'    */
    /* second call:     Ch_1_Par_Val == 'A', Ch_2_Par_Val == 'C'    */
    /* third call:      Ch_1_Par_Val == 'B', Ch_2_Par_Val == 'C'    */

Capital_Letter   Ch_1_Par_Val;
Capital_Letter   Ch_2_Par_Val;
{
  Capital_Letter        Ch_1_Loc;
  Capital_Letter        Ch_2_Loc;

  Ch_1_Loc = Ch_1_Par_Val;
  Ch_2_Loc = Ch_1_Loc;
  if (Ch_2_Loc != Ch_2_Par_Val)
    /* then, executed */
    return (Ident_1);
  else  /* not executed */
  {
    Ch_1_Glob = Ch_1_Loc;
    return (Ident_2);
   }
} /* Func_1 */


Boolean Func_2 (Str_1_Par_Ref, Str_2_Par_Ref)
/*************************************************/
    /* executed once */
    /* Str_1_Par_Ref == "DHRYSTONE PROGRAM, 1'ST STRING" */
    /* Str_2_Par_Ref == "DHRYSTONE PROGRAM, 2'ND STRING" */

Str_30  Str_1_Par_Ref;
Str_30  Str_2_Par_Ref;
{
  REG One_Thirty        Int_Loc;
      Capital_Letter    Ch_Loc;

  Int_Loc = 2;
  while (Int_Loc <= 2) /* loop body executed once */
    if (Func_1 (Str_1_Par_Ref[Int_Loc],
                Str_2_Par_Ref[Int_Loc+1]) == Ident_1)
      /* then, executed */
    {
      Ch_Loc = 'A';
      Int_Loc += 1;
    } /* if, while */
  if (Ch_Loc >= 'W' && Ch_Loc < 'Z')
    /* then, not executed */
    Int_Loc = 7;
  if (Ch_Loc == 'R')
    /* then, not executed */
    return (true);
  else /* executed */
  {
    if (strcmp (Str_1_Par_Ref, Str_2_Par_Ref) > 0)
      /* then, not executed */
    {
      Int_Loc += 7;
      Int_Glob = Int_Loc;
      return (true);
    }
    else /* executed */
      return (false);
  } /* if Ch_Loc */
} /* Func_2 */


Boolean Func_3 (Enum_Par_Val)
/***************************/
    /* executed once        */
    /* Enum_Par_Val == Ident_3 */
Enumeration Enum_Par_Val;
{
  Enumeration Enum_Loc;

  Enum_Loc = Enum_Par_Val;
  if (Enum_Loc == Ident_3)
    /* then, executed */
    return (true);
  else /* not executed */
    return (false);
} /* Func_3 */



///////////////////////////////////////////////////////

/* Global Variables: */
Rec_Pointer	Ptr_Glob, Next_Ptr_Glob;
int		Int_Glob;
Boolean	Bool_Glob;
char	Ch_1_Glob, Ch_2_Glob;
int		Arr_1_Glob[50];
int		Arr_2_Glob[50][50];

Enumeration		Func_1 ();
/* forward declaration necessary since Enumeration may not simply be int */
#define REG
#ifndef REG
	Boolean Reg = false;
#else
	Boolean Reg = true;
#endif

unsigned long long Begin_Time,End_Time,User_Time;
unsigned long long	Microseconds_Per_Dhrystone, Dhrystones_Per_Second;
 /* end of variables for time measurement */                                        
                                                                                    
 main_dhry()
 /*****/                                                                            
{
	One_Fifty       Int_1_Loc;
	REG   One_Fifty       Int_2_Loc;
	One_Fifty       Int_3_Loc;
	REG   char            Ch_Index;
	Enumeration     Enum_Loc;
	Str_30          Str_1_Loc;
	Str_30          Str_2_Loc;
	REG   int       Run_Index;
	REG   int        Number_Of_Runs;
	unsigned long long time1,time2;
	unsigned int 	Real_Time;
	float DMIPS;
	float DMIPS_MHZ;
	int error_num=0;

   /* Initializations */
   Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));                        
   Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));                             
                                                                                    
   Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;                           
   Ptr_Glob->Discr                       = Ident_1;                                 
   Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;                                 
   Ptr_Glob->variant.var_1.Int_Comp      = 40;                                      
   strcpy (Ptr_Glob->variant.var_1.Str_Comp,                                        
           "DHRYSTONE PROGRAM, SOME STRING");                                       
   strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");                            
                                                                                    
   Arr_2_Glob[8][7] = 10;
                                                                                    
	PSPRINTF ("\n");
	PSPRINTF ("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
	PSPRINTF ("\n");
   
   if (Reg)                                                                         
   {                                                                                
     PSPRINTF ("Program compiled with 'register' attribute\n");
     PSPRINTF ("\n");
   }                                                                                
   else                                                                             
   {                                                                                
     PSPRINTF ("Program compiled without 'register' attribute\n");
     PSPRINTF ("\n");
   }                                                                                
   //dhryPrintf ("Please give the number of runs through the benchmark: ");
   {                                                                                
//	   int n = 10000000;
//	   int n = 1000000;
	   int n = 10000;

     //scanf ("%d", &n);
     Number_Of_Runs = n;                                                            
   }                                                                                
   PSPRINTF("\n");
                                                                                    
   PSPRINTF("Execution starts, %d runs through Dhrystone\n", Number_Of_Runs);                                                                                
   /***************/                                                                
   /* Start timer */                                                                
   /***************/
   time1 =get_tb_f();
   for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)                    
   {
     Proc_5();                                                                      
     Proc_4();                                                                      
       /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */                  
     Int_1_Loc = 2;                                                                 
     Int_2_Loc = 3;                                                                 
     strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");                          
     Enum_Loc = Ident_2;                                                            
     Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);                                   
       /* Bool_Glob == 1 */                                                         
     while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */                   
     {                                                                              
       Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;                                       
         /* Int_3_Loc == 7 */                                                       
       Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);                                   
         /* Int_3_Loc == 7 */                                                       
       Int_1_Loc += 1;                                                              
     } /* while */                                                                  
       /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */                         
     Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);                         
       /* Int_Glob == 5 */                                                          
     Proc_1 (Ptr_Glob);                                                             
     for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)                        
                              /* loop body executed twice */                        
     {                                                                              
       if (Enum_Loc == Func_1 (Ch_Index, 'C'))                                      
           /* then, not executed */                                                 
         {                                                                          
         Proc_6 (Ident_1, &Enum_Loc);                                               
         strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");                      
         Int_2_Loc = Run_Index;                                                     
         Int_Glob = Run_Index;                                                      
         }                                                                          
     }                                                                              
       /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */                         
     Int_2_Loc = Int_2_Loc * Int_1_Loc;                                             
     Int_1_Loc = Int_2_Loc / Int_3_Loc;                                             
     Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;                           
       /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */                        
     Proc_2 (&Int_1_Loc);                                                           
       /* Int_1_Loc == 5 */                                                         
                                                                                    
   } /* loop "for Run_Index" */                                                     
                                                                                    
   /**************/                                                                 
   /* Stop timer */                                                                 
   /**************/                                                                 
   time2 =get_tb_f();
  
   PSPRINTF("Execution ends\n");
   PSPRINTF("\n");
   PSPRINTF ("Final values of the variables used in the benchmark:\n");
   PSPRINTF ("\n");
   PSPRINTF ("Int_Glob:            %d\n", Int_Glob);
   PSPRINTF ("        should be:   %d\n", 5);
   PSPRINTF ("Bool_Glob:           %d\n", Bool_Glob);
   PSPRINTF ("        should be:   %d\n", 1);
   PSPRINTF ("Ch_1_Glob:           %d\n", Ch_1_Glob);
   PSPRINTF ("        should be:   %d\n", 'A');
   PSPRINTF ("Ch_2_Glob:           %d\n", Ch_2_Glob);
   PSPRINTF ("        should be:   %d\n", 'B');
   PSPRINTF ("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
   PSPRINTF ("        should be:   %d\n", 7);
   PSPRINTF ("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
   PSPRINTF ("        should be:   %d + 10\n",Number_Of_Runs);
   PSPRINTF ("Ptr_Glob->\n");
   //PSPRINTF ("  Ptr_Comp:          %d\n", (int) Ptr_Glob->Ptr_Comp);
   //PSPRINTF ("        should be:   (implementation-dependent)\n");
   PSPRINTF ("  Discr:             %d\n", Ptr_Glob->Discr);
   PSPRINTF ("        should be:   %d\n", 0);
   PSPRINTF ("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
   PSPRINTF ("        should be:   %d\n", 2);
   PSPRINTF ("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
   PSPRINTF ("        should be:   %d\n", 17);
   PSPRINTF ("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
   PSPRINTF ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
   PSPRINTF ("Next_Ptr_Glob->\n");
   //PSPRINTF ("  Ptr_Comp:          %d\n", (int) Next_Ptr_Glob->Ptr_Comp);
   //PSPRINTF ("        should be:   (implementation-dependent), same as above\n");
   PSPRINTF ("  Discr:             %d\n", Next_Ptr_Glob->Discr);
   PSPRINTF ("        should be:   %d\n", 0);
   PSPRINTF ("  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
   PSPRINTF ("        should be:   %d\n", 1);
   PSPRINTF ("  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
   PSPRINTF ("        should be:   %d\n", 18);
   PSPRINTF ("  Str_Comp:          %s\n",
                                 Next_Ptr_Glob->variant.var_1.Str_Comp);
   PSPRINTF ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
   PSPRINTF ("Int_1_Loc:           %d\n", Int_1_Loc);
   PSPRINTF ("        should be:   %d\n", 5);
   PSPRINTF ("Int_2_Loc:           %d\n", Int_2_Loc);
   PSPRINTF ("        should be:   %d\n", 13);
   PSPRINTF ("Int_3_Loc:           %d\n", Int_3_Loc);
   PSPRINTF ("        should be:   %d\n", 7);
   PSPRINTF ("Enum_Loc:            %d\n", Enum_Loc);
   PSPRINTF ("        should be:   %d\n", 1);
   PSPRINTF ("Str_1_Loc:           %s\n", Str_1_Loc);
   PSPRINTF ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
   PSPRINTF ("Str_2_Loc:           %s\n", Str_2_Loc);
   PSPRINTF ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
   PSPRINTF ("\n");

    error_num=0;
	if (Int_Glob != 5) error_num=1;
	if (Bool_Glob != 1) error_num=2;
	if (Ch_1_Glob != 'A') error_num++;
	if (Ch_2_Glob != 'B') error_num++;
	if (Arr_1_Glob[8] != 7) error_num++;
	if (Arr_2_Glob[8][7] != Number_Of_Runs + 10) error_num++;
	if (Ptr_Glob->Discr != 0) error_num++;
	if (Ptr_Glob->variant.var_1.Enum_Comp != 2) error_num++;
	if (Ptr_Glob->variant.var_1.Int_Comp != 17) error_num++;
	if (strcmp(Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING")) error_num++;
	if (Next_Ptr_Glob->Discr != 0) error_num++;
	if (Next_Ptr_Glob->variant.var_1.Enum_Comp != 1) error_num++;
	if (Next_Ptr_Glob->variant.var_1.Int_Comp != 18) error_num++;
	if (strcmp(Next_Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING")) error_num++;
	if (Int_1_Loc != 5) error_num++;
	if (Int_2_Loc != 13) error_num++;
	if (Int_3_Loc != 7) error_num++;
	if (Enum_Loc != 1) error_num++;
	if (strcmp(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING")) error_num++;
	if (strcmp(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING")) error_num++;

	if(error_num == 0)
	{
		PSPRINTF ("Benchmark exec ok.\n");
		PSPRINTF ("\n");
	}
	else
	{
		PSPRINTF ("Benchmark exec err.\n");
		PSPRINTF ("\n");
	}


   User_Time = time2-time1;
   Real_Time =User_Time/HZ;
   Microseconds_Per_Dhrystone = (float)(User_Time * Mic_secs_Per_Second / ((unsigned long long) HZ * ((unsigned long long) Number_Of_Runs)));
   Dhrystones_Per_Second = ((float)(HZ * (unsigned long long) Number_Of_Runs) / (unsigned long long) User_Time);
   //DMIPS = Iterations / (Runtime[Sec]*CLK[MHz]*1757)
   DMIPS = (float)Dhrystones_Per_Second/1757;
   DMIPS_MHZ =(float)DMIPS/MHZ;

   //PSPRINTF ("Real_Time:           %f S\n", Real_Time);
   //PSPRINTF ("Microseconds_Per_Dhrystone:           %f uS\n", Microseconds_Per_Dhrystone);
   //PSPRINTF ("Dhrystones_Per_Second:           %f\n", Dhrystones_Per_Second);
   PSPRINTF ("DMIPS:           %f\n", DMIPS);   PSPRINTF ("DMIPS_MHZ:           %f\n", DMIPS_MHZ);
   //{
 //#ifdef TIME
  //   Microseconds = (float) User_Time * Mic_secs_Per_Second
  //                       / (float) Number_Of_Runs;
  //   Dhrystones_Per_Second = (float) Number_Of_Runs / (float) User_Time;
 //#else
 //    Microseconds = (float) User_Time * Mic_secs_Per_Second
 //                        / ((float) HZ * ((float) Number_Of_Runs));
 //    Dhrystones_Per_Second = ((float) HZ * (float) Number_Of_Runs)
 //                        / (float) User_Time;
 //#endif
   //printf ("Microseconds for one run through Dhrystone: ");
  // printf ("%6.1f \n", Microseconds);
  // printf ("Dhrystones per Second:                      ");
  // printf ("%6.1f \n", Dhrystones_Per_Second);
  // printf ("\n");
  // printf ("DMIPS : %8.3f \n", DMIPS);
   //}
   free(Next_Ptr_Glob);                        
   free(Ptr_Glob);
 }                                                                                  
                                                                                    
                                                                                    
 Proc_1 (Ptr_Val_Par)                                                               
 /******************/                                                               
                                                                                    
 REG Rec_Pointer Ptr_Val_Par;                                                       
     /* executed once */                                                            
 {                                                                                  
   REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;                             
                                         /* == Ptr_Glob_Next */                     
   /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */                 
   /* corresponds to "rename" in Ada, "with" in Pascal           */                 
                                                                                    
   structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);                                
   Ptr_Val_Par->variant.var_1.Int_Comp = 5;                                         
   Next_Record->variant.var_1.Int_Comp                                              
         = Ptr_Val_Par->variant.var_1.Int_Comp;                                     
   Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;                                   
   Proc_3 (&Next_Record->Ptr_Comp);                                                 
     /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp                                             
                         == Ptr_Glob->Ptr_Comp */                                   
   if (Next_Record->Discr == Ident_1)                                               
     /* then, executed */                                                           
   {                                                                                
     Next_Record->variant.var_1.Int_Comp = 6;                                       
     Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp,                                  
            &Next_Record->variant.var_1.Enum_Comp);                                 
     Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;                                    
     Proc_7 (Next_Record->variant.var_1.Int_Comp, 10,                               
            &Next_Record->variant.var_1.Int_Comp);                                  
   }                                                                                
   else /* not executed */                                                          
     structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);                           
 } /* Proc_1 */                                                                     
                                                                                    
                                                                                    
 Proc_2 (Int_Par_Ref)                                                               
 /******************/                                                               
     /* executed once */                                                            
     /* *Int_Par_Ref == 1, becomes 4 */                                             
                                                                                    
 One_Fifty   *Int_Par_Ref;                                                          
 {                                                                                  
   One_Fifty  Int_Loc;                                                              
   Enumeration   Enum_Loc;                                                          
                                                                                    
   Int_Loc = *Int_Par_Ref + 10;                                                     
   do /* executed once */                                                           
     if (Ch_1_Glob == 'A')                                                          
       /* then, executed */                                                         
     {                                                                              
       Int_Loc -= 1;                                                                
       *Int_Par_Ref = Int_Loc - Int_Glob;                                           
       Enum_Loc = Ident_1;                                                          
     } /* if */                                                                     
   while (Enum_Loc != Ident_1); /* true */                                          
 } /* Proc_2 */                                                                     
                                                                                    
                                                                                    
 Proc_3 (Ptr_Ref_Par)                                                               
 /******************/                                                               
     /* executed once */                                                            
     /* Ptr_Ref_Par becomes Ptr_Glob */                                             
                                                                                    
 Rec_Pointer *Ptr_Ref_Par;                                                          
                                                                                    
 {                                                                                  
   if (Ptr_Glob != Null)                                                            
     /* then, executed */                                                           
     *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;                                             
   Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);                        
 } /* Proc_3 */                                                                     
                                                                                    
                                                                                    
 Proc_4 () /* without parameters */                                                 
 /*******/                                                                          
     /* executed once */                                                            
 {                                                                                  
   Boolean Bool_Loc;                                                                
                                                                                    
   Bool_Loc = Ch_1_Glob == 'A';                                                     
   Bool_Glob = Bool_Loc | Bool_Glob;                                                
   Ch_2_Glob = 'B';                                                                 
 } /* Proc_4 */                                                                     
                                                                                    
                                                                                    
 Proc_5 () /* without parameters */                                                 
 /*******/                                                                          
     /* executed once */                                                            
 {                                                                                  
   Ch_1_Glob = 'A';                                                                 
   Bool_Glob = false;                                                               
 } /* Proc_5 */                                                                     

