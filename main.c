#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <cairo.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//1. Define dimensions of output image
#define NUMBEROFIMAGES 8
#define WIDTH 50.0
#define FULLCIRCLERADIANS 6.28319

void *testThread(void* arguments){
    int arg = *((int*)arguments);
    printf("THREAD: %d Started\n",arg);
    sleep(3);
    printf("THREAD: %d  Ended\n",arg);
}

void *cairoDraw(void* arguments) {

    int arg = *((int*)arguments);
    double radius = WIDTH / 2;
    double start = radius;
    double x = start + (radius * arg);
    char numberCar = arg;
    char targetString[18];
    int success = sprintf(targetString, "./cairopng-%d.png", numberCar);
    printf("%s %d %d\n",targetString, success, arg);

    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, (WIDTH * NUMBEROFIMAGES / 2) - 1, WIDTH);
    cr = cairo_create (surface);

    //Fill White Background
    cairo_set_source_rgb (cr, 255, 255, 255);
    cairo_rectangle(cr,0,0,WIDTH * NUMBEROFIMAGES, WIDTH);
    cairo_fill (cr);

    //Draw Circle
    cairo_set_source_rgb (cr, 255, 0, 0);
    cairo_arc(cr,x,radius,radius,0,FULLCIRCLERADIANS);
    cairo_fill (cr);

    cairo_status_t writeSTATUS = cairo_surface_write_to_png(surface,targetString);

    printf("cairo write status: %d %s\n",writeSTATUS,cairo_status_to_string(writeSTATUS));

    printf("Thread %d wrote to %s\n",arg,targetString);
}

int main(int argc, const char** argv) {

    printf("%d %s\n",argc, argv);
    int singleThreaded = 0;

    if ( argc > 1 ) {

        const char *fistArg = argv[1];
        if (strstr(fistArg, "-s")) {
            printf("asked to run single threaded\n");
            singleThreaded = 1;
        }

    } else {
        printf("will run multi threaded\n");
    }


    clock_t start, end;
    double cpu_time_used;
    start = clock();


    int numberOfImages = NUMBEROFIMAGES - 1;
    int thread_args[NUMBEROFIMAGES];

    if (singleThreaded == 0) {

        pthread_t *threads[NUMBEROFIMAGES];
        pthread_t *currentThread;

        int resultCode, joinSuccess;

        for (int i = 0; i < numberOfImages; i++) {

            thread_args[i] = i;
            //3. use threads to generate images with cairo
            resultCode = pthread_create(&threads[i], NULL, cairoDraw, &thread_args[i]);
            printf("threads created %d\n", thread_args[i]);
            assert(!resultCode);

        }

        for (int i = 0; i < numberOfImages; i++) {
            currentThread = threads[i];

            //4. LInk threads to ensure all complete before exiting
            joinSuccess = pthread_join(currentThread, NULL);
            assert(!joinSuccess);
        }


    } else {

        //call function iteratively on the main thread
        for (int i = 0; i < numberOfImages; i++) {

            printf("creating image %d on main thread\n",i);
            thread_args[i] = i;
            cairoDraw(&thread_args[i]);

        }

    }

    const char* processType = (singleThreaded) ? "Single Threaded" : "Multi Threaded";

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("done, in %fs (%s)", cpu_time_used, processType);

    return EXIT_SUCCESS;
}
