# Threaded Circles

![Alt text](/repo/bounce.gif?raw=true "Screenshot")

### C, Cairo and pthread

I wanted to see how much faster cairo could produce simple images when threaded vs iterated on the main thread. 

### Single Threaded

done, in 0.007900s (Single Threaded)

### Multi Threaded

done, in 0.024525s (Multi Threaded)

### Result

So the single threaded execution takes approximately one third of the time of the threaded version. This must be due to the overhead of creating new pthreads and waiting for the completion on each one, rather than just blasting ahead with the next iteration.

#### Libraries Used

`stdio.h` - input and output

`stdlib.h` - signals
`pthread.h` - threading
`assert.h` - error checking
`cairo.h` - drawing
`unistd.h` - posix APIs e.g "sleep"
`string.h` - strings
`time.h` - measuring the length of execution

If you need Cairo, get it from https://cairographics.org/download/ 

