/***************************************************************/
/*                                                             */
/* lizard.c                                                    */
/*                                                             */
/* To compile, you need all the files listed below             */
/*   lizard.c                                                  */
/*                                                             */
/* Be sure to use the -lpthread option for the compile command */
/*   gcc -g -Wall lizard.c -o lizard -lpthread                 */
/*                                                             */
/* Execute with the -d command-line option to enable debugging */
/* output.  For example,                                       */
/*   ./lizard -d                                               */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#ifdef __APPLE__
#include <mach/semaphore.h>
#include <mach/task.h>
#define sem_init(a,b,c)     semaphore_create(mach_task_self(), (semaphore_t *)a, SYNC_POLICY_FIFO, c)
#define sem_destroy(a)      semaphore_destroy(mach_task_self(), *((semaphore_t *)a))
#define sem_post(a)         semaphore_signal(*((semaphore_t *)a))
#define sem_wait(a)         semaphore_wait(*((semaphore_t *)a))
#define sem_t               semaphore_t
#else
#include <semaphore.h>
#endif

/*
 * This is a stub file.  It contains very little code and what
 * it does contain may need to be altered or removed.  It is
 * only provided for a starting point.
 *
 * The comments are probably useful.
 */

/*
 * Function prototypes go here
 * You may need more functions, but I don't think so
 */
void * lizardThread( void * param );

/*Added these functions to make reading and debugging code easier.*/
/*Function descriptions are available in the text above each function*/
/*Schneider Sherrell*/
void m2sago_lock();
void m2sago_unlock();
void sago2m_lock();
void sago2m_unlock();


/*
 * Define "constant" values here
 */


/*
 * Make this 1 to check for lizards traving in both directions
 * Leave it 0 to allow bidirectional travel
 */
#define UNIDIRECTIONAL       1

/*
 * Set this to the number of seconds you want the lizard world to
 * be simulated.
 * Try 30 for development and 120 for more thorough testing.
 */
#define WORLDEND             30

/*
 * Number of lizard threads to create
 */
#define NUM_LIZARDS          20

/*
 * Maximum lizards crossing at once before alerting cats
 */
#define MAX_LIZARD_CROSSING  4

/*
 * Maximum seconds for a lizard to sleep
 */
#define MAX_LIZARD_SLEEP     3

/*
 * Maximum seconds for a lizard to eat
 */
#define MAX_LIZARD_EAT       5

/*
 * Number of seconds it takes to cross the driveway
 */
#define CROSS_SECONDS        2



/*
 * Declare global variables here
 */


sem_t road_sem;
pthread_mutex_t mute;

/*Pthread locks*/
pthread_cond_t wait_add, wait_sago2monk, wait_monk2sago;



/**************************************************/
/* Please leave these variables alone.  They are  */
/* used to check the proper functioning of your   */
/* program.  They should only be used in the code */
/* I have provided.                               */
/**************************************************/
int numCrossingSago2MonkeyGrass;
int numCrossingMonkeyGrass2Sago;
int debug;
int running;
/**************************************************/









/*
 * main()
 *
 * Should initialize variables, locks, semaphores, etc.
 * Should start the cat thread and the lizard threads
 * Should block until all threads have terminated
 * Status: Incomplete - Make changes to this code.
 */
int main(int argc, char **argv)
{
    /*
     * Declare local variables
     */

    int i,j;
    pthread_t lizards[NUM_LIZARDS];

    /*
     * Check for the debugging flag (-d)
     */
    debug = 0;
    if (argc > 1)
    if (strncmp(argv[1], "-d", 2) == 0)
        debug = 1;


    /*
     * Initialize variables
     */
    numCrossingSago2MonkeyGrass = 0;
    numCrossingMonkeyGrass2Sago = 0;
    running = 1;


    /*
     * Initialize random number generator
     */
    srandom( (unsigned int)time(NULL) );


    /*
     * Initialize locks and/or semaphores Schneider Sherrell
     */


    sem_init(&road_sem, 0, MAX_LIZARD_CROSSING);
    if(UNIDIRECTIONAL){
        pthread_mutex_init(&wait_monk2sago, NULL);
        pthread_mutex_init(&wait_sago2monk, NULL);
    }
    else
        pthread_mutex_init(&wait_add, NULL);


    /*
     * Create NUM_LIZARDS lizard threads
     */

    j = 1;
    for( i = 0; i < NUM_LIZARDS; i++){
        pthread_create(&lizards[i], NULL, &lizardThread, (void *)(intptr_t)j);
        j++;
    }




    /*
     * Now let the world run for a while
     */

    sleep( WORLDEND );


    /*
     * That's it - the end of the world
     */

    running = 0;


    /*
     * Wait until all threads terminate
     */



    for(i = 0; i < NUM_LIZARDS; i++)
        pthread_join(lizards[i], NULL);


    /*
     * Delete the locks and semaphores
     */

    /*Schneider Sherrell*/
    sem_destroy(&road_sem);
    if(UNIDIRECTIONAL){
        pthread_mutex_destroy(&wait_monk2sago);
        pthread_mutex_destroy(&wait_sago2monk);
    }
    else
        pthread_mutex_destroy(&wait_add);

    /*
     * Exit happily
     */
    return 0;
}


/*
 * These prototypes are declared here so that main()
 * can't use them directly.  Functions and variables
 * must be declared before they can be used.  Using them
 * below this point is fine.
 */

void lizard_sleep(int num);
void sago_2_monkeyGrass_is_safe(int num);
void cross_sago_2_monkeyGrass(int num);
void made_it_2_monkeyGrass(int num);
void lizard_eat(int num);
void monkeyGrass_2_sago_is_safe(int num);
void cross_monkeyGrass_2_sago(int num);
void made_it_2_sago(int num);





/*
 * lizardThread()
 *
 * Follows the algorithm provided in the assignment
 * description to simulate lizards crossing back and forth
 * between a sago palm and some monkey grass.
 * input: lizard number
 * output: N/A
 * Status: Incomplete - Make changes as you see are necessary.
 */
void * lizardThread( void * param ) {
    int num = (intptr_t)param;

    if (debug)
    {
        printf("[%2d] lizard is alive\n", num);
        fflush(stdout);
    }

    while(running) {
        /*
         * Follow the algorithm given in the assignment
         * using calls to the functions declared above.
         * You'll need to complete the implementation of
         * some functions by filling in the code.  Some
         * are already completed - see the comments.
         */
        lizard_sleep(num);

        /*Schneider Sherrell*/
        /*Check if safe, then cross, then report if safely made it*/
        sago_2_monkeyGrass_is_safe(num);
        cross_sago_2_monkeyGrass(num);
        made_it_2_monkeyGrass(num);

        lizard_eat(num);

        /*Check if safe, then cross, then report if safely made it*/
        monkeyGrass_2_sago_is_safe(num);
        cross_monkeyGrass_2_sago(num);
        made_it_2_sago(num);
    }



    pthread_exit(NULL);
}


/*
 * lizard_sleep()
 *
 * Simulate a lizard sleeping for a random amount of time
 * input: lizard number
 * output: N/A
 * Status: Completed - No need to change any of this code.
 */
void lizard_sleep(int num)
{
    int sleepSeconds;

    sleepSeconds = 1 + (int)(random() / (double)RAND_MAX * MAX_LIZARD_SLEEP);

    if (debug)
    {
        printf( "[%2d] sleeping for %d seconds\n", num, sleepSeconds );
        fflush( stdout );
    }

    sleep( sleepSeconds );

    if (debug)
    {
        printf( "[%2d] awake\n", num );
        fflush( stdout );
    }
}


/*
 * sago_2_monkeyGrass_is_safe()
 *
 * Returns when it is safe for this lizard to cross from the sago
 * to the monkey grass.   Should use some synchronization
 * facilities (lock/semaphore) here.
 * input: lizard number
 * output: N/A
 */
void sago_2_monkeyGrass_is_safe(int num)
{
    if (debug)
    {
        printf( "[%2d] checking  sago -> monkey grass\n", num );
        fflush( stdout );
    }


    /*Try to acquire lock  Schneider Sherrell*/
    sem_wait(&road_sem);


    if (debug)
    {
        printf( "[%2d] thinks  sago -> monkey grass  is safe\n", num );
        fflush( stdout );
    }
}


/*
 * cross_sago_2_monkeyGrass()
 *
 * Delays for 1 second to simulate crossing from the sago to
 * the monkey grass.
 * input: lizard number
 * output: N/A
 */
void cross_sago_2_monkeyGrass(int num)
{
    if (debug)
    {
        printf( "[%2d] crossing  sago -> monkey grass\n", num );
        fflush( stdout );
    }
    /*
     * One more crossing this way
     */


    /*Call lock function  Schneider Sherrell*/
    sago2m_lock();



    /*
     * Check for too many lizards crossing
     */
    if (numCrossingSago2MonkeyGrass + numCrossingMonkeyGrass2Sago > MAX_LIZARD_CROSSING)
    {
        printf( "\tThe cats are happy - they have toys.\n" );
        printf( "\t%d crossing sago -> monkey grass\n", numCrossingSago2MonkeyGrass );
        exit( -1 );
    }

    /*
     * Check for lizards cross both ways
     */

    if (numCrossingMonkeyGrass2Sago && UNIDIRECTIONAL)
    {
        printf( "\tCrash!  We have a pile-up on the concrete.\n" );
        printf( "\t%d crossing sago -> monkey grass\n", numCrossingSago2MonkeyGrass );
        printf( "\t%d crossing monkey grass -> sago\n", numCrossingMonkeyGrass2Sago );
        exit( -1 );
    }

    /*
     * It takes a while to cross, so simulate it
     */
    sleep( CROSS_SECONDS );

    /*
     * That one seems to have made it
     */



    sago2m_unlock();

}


/*
 * made_it_2_monkeyGrass()
 *
 * Tells others they can go now
 * input: lizard number
 * output: N/A
 * Status: Incomplete - Make changes as you see are necessary.
 */
void made_it_2_monkeyGrass(int num)
{

    /*Increment semaphore lock. Schneider Sherrell*/
    sem_post(&road_sem);

    /*
     * Whew, made it across
     */
    if (debug)
    {
        printf( "[%2d] made the  sago -> monkey grass  crossing\n", num );
        fflush( stdout );
    }


}


/*
 * lizard_eat()
 *
 * Simulate a lizard eating for a random amount of time
 * input: lizard number
 * output: N/A
 * Status: Completed - No need to change any of this code.
 */
void lizard_eat(int num)
{
    int eatSeconds;

    eatSeconds = 1 + (int)(random() / (double)RAND_MAX * MAX_LIZARD_EAT);

    if (debug)
    {
        printf( "[%2d] eating for %d seconds\n", num, eatSeconds );
        fflush( stdout );
    }

    /*
     * Simulate eating by blocking for a few seconds
     */
    sleep( eatSeconds );

    if (debug)
    {
        printf( "[%2d] finished eating\n", num );
        fflush( stdout );
    }
}


/*
 * monkeyGrass_2_sago_is_safe()
 *
 * Returns when it is safe for this lizard to cross from the monkey
 * grass to the sago.   Should use some synchronization
 * facilities (lock/semaphore) here.
 * input: lizard number
 * output: N/A
 * Status: Incomplete - Make changes as you see are necessary.
 */
void monkeyGrass_2_sago_is_safe(int num)
{
    if (debug)
    {
        printf( "[%2d] checking  monkey grass -> sago\n", num );
        fflush( stdout );
    }


    /*See if crossing is safe. Schneider Sherrell*/
    sem_wait(&road_sem);



    if (debug)
    {
        printf( "[%2d] thinks  monkey grass -> sago  is safe\n", num );
        fflush( stdout );
    }
}



/*
 * cross_monkeyGrass_2_sago()
 *
 * Delays for 1 second to simulate crossing from the monkey
 * grass to the sago.
 * input: lizard number
 * output: N/A
 */
void cross_monkeyGrass_2_sago(int num) {
    if (debug) {
        printf("[%2d] crossing  monkey grass -> sago\n", num);
        fflush(stdout);
    }



    /*
     * One more crossing this way
     */

    /*Call lock function Schneider Sherrell*/
    m2sago_lock();


    /*
     * Check for too many lizards crossing
     */
    if (numCrossingMonkeyGrass2Sago + numCrossingSago2MonkeyGrass > MAX_LIZARD_CROSSING) {
        printf("\tThe cats say yum! - they love lizards.\n");
        printf("\t%d crossing monkey grass -> sago\n", numCrossingMonkeyGrass2Sago);
        exit(-1);
    }

    /*
    * Check for lizards cross both ways
    */
    if (numCrossingSago2MonkeyGrass && UNIDIRECTIONAL) {
        printf("\tOh No!, the lizards have cats all over them.\n");
        printf("\t%d crossing sago -> monkey grass\n", numCrossingSago2MonkeyGrass);
        printf("\t%d crossing monkey grass -> sago\n", numCrossingMonkeyGrass2Sago);
        exit(-1);
    }

    /*
     * It takes a while to cross, so simulate it
     */
    sleep(CROSS_SECONDS);

    /*
     * That one seems to have made it
     */


    m2sago_unlock(); /*Finished crossing. Call unlock function Schneider Sherrell*/


}


/*
 * made_it_2_sago()
 *
 * Tells others they can go now
 * input: lizard number
 * output: N/A
 */
void made_it_2_sago(int num)
{

    /*Schneider Sherrell Increment Semaphore*/
    sem_post(&road_sem);


    /*
     * Whew, made it across
     */
    if (debug)
    {
        printf( "[%2d] made the  monkey grass -> sago  crossing\n", num );
        fflush( stdout );
    }



}





/* Schneider Sherrell
 * m2sago_lock()
 *
 * Locks the counter, increments, and executes a lizard crossing or waits if
 * needed.
 * input: none
 * output: none
 */
void m2sago_lock(){



    /*Acquire lock*/
    pthread_mutex_lock(&mute);


    /*Code for unidirectional movement*/
    if (UNIDIRECTIONAL) {
        while (numCrossingSago2MonkeyGrass > 0 || numCrossingMonkeyGrass2Sago + numCrossingSago2MonkeyGrass == MAX_LIZARD_CROSSING)
            pthread_cond_wait(&wait_monk2sago, &mute);
        numCrossingMonkeyGrass2Sago++;
        if (numCrossingMonkeyGrass2Sago < MAX_LIZARD_CROSSING)

            /*Wake all waiting lizards*/
            pthread_cond_broadcast(&wait_monk2sago);
    }

        /*Code for bi-directional movement*/
    else {
        while (numCrossingMonkeyGrass2Sago + numCrossingSago2MonkeyGrass >= MAX_LIZARD_CROSSING)
            pthread_cond_wait(&wait_add, &mute);
        numCrossingMonkeyGrass2Sago++;
        //If more room, signal another waiting adder.
        if (numCrossingMonkeyGrass2Sago + numCrossingSago2MonkeyGrass < MAX_LIZARD_CROSSING)
            pthread_cond_broadcast(&wait_add);
    }

    /*Release lock*/
    pthread_mutex_unlock(&mute);
}



/* Schneider Sherrell
 * m2sago_unlock()
 *
 * Locks the counter, decrements, and signals any waiting lizards
 * input: none
 * output: none
 */
void m2sago_unlock(){

    /*Acquire lock for counter.*/
    pthread_mutex_lock(&mute);
    numCrossingMonkeyGrass2Sago--;

    /*Code for UNIDIRECTIONAL movement*/
    if (UNIDIRECTIONAL) {
        if (numCrossingMonkeyGrass2Sago == 0)
            pthread_cond_broadcast(&wait_sago2monk);
    }

        /*Code for BIDIRECTIONAL movement*/
    else
        pthread_cond_broadcast(&wait_add);


    /*Release lock*/
    pthread_mutex_unlock(&mute);



}





/* Schneider Sherrell
 * sago2m_lock()
 *
 * Locks the counter, increments, and executes a lizard crossing or waits if
 * needed.
 * input: none
 * output: none
 */
void sago2m_lock(){
    /**** // Acquire lock // ****/
    pthread_mutex_lock(&mute);



    /*Code for UNIDIRECTIONAL movement*/
    if(UNIDIRECTIONAL) {

        while (numCrossingMonkeyGrass2Sago > 0 || numCrossingMonkeyGrass2Sago + numCrossingSago2MonkeyGrass == MAX_LIZARD_CROSSING)
            pthread_cond_wait(&wait_sago2monk, &mute);

        numCrossingSago2MonkeyGrass++;

        /*Wake all waiting lizards*/
        if (numCrossingSago2MonkeyGrass < MAX_LIZARD_CROSSING)
            pthread_cond_broadcast(&wait_sago2monk);
    }



        /*Code for BIDIRECTIONAL movement*/
    else{
        while(numCrossingMonkeyGrass2Sago + numCrossingSago2MonkeyGrass >= MAX_LIZARD_CROSSING)
            pthread_cond_wait(&wait_add, &mute);

        numCrossingSago2MonkeyGrass++;

        /*If more adders waiting, signal if safe to cross*/
        if(numCrossingMonkeyGrass2Sago + numCrossingSago2MonkeyGrass < MAX_LIZARD_CROSSING)
            pthread_cond_broadcast(&wait_add);
    }

    /*Release lock*/
    pthread_mutex_unlock(&mute);
}





/* Schneider Sherrell
 * sago2m_unlock()
 *
 * Locks the counter, decrements, and signals any waiting lizards.
 * input: none
 * output: none
 */
void sago2m_unlock(){
    /*Acquire lock*/
    pthread_mutex_lock(&mute);

    /*Decrement counter*/
    numCrossingSago2MonkeyGrass--;


    /*Code for UNIDIRECTIONAL movement*/
    if(UNIDIRECTIONAL) {
        if (numCrossingSago2MonkeyGrass == 0)
            /*If none crossing opposite direction, safe to wake all*/
            pthread_cond_broadcast(&wait_monk2sago);
    }


    /*Code for BIDIRECTIONAL movement*/
    else
        /*Wake a thread if waiting*/
        pthread_cond_broadcast(&wait_add);

    /*Release lock*/
    pthread_mutex_unlock(&mute);
}
