//Using SDL and standard IO
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>

//Constants
const double PI = 3.14159265358979323846;

//Functions
void colorTreatment(SDL_Surface *image, int value);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format, int value);


Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

// get the pixel data depending on the format used
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}


// Put the given pixel in the surface at the coordinates (x,y)
// Adapting to the format of the image
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

// Simple function applying a treatment to every pixel in an image
// in this case it is a Blackscale treatment and directly modify the image
void colorTreatment(SDL_Surface *image, int value)
{
    int i, j;
    SDL_LockSurface(image);
    int h = image->h;
    int w = image->w;
    SDL_PixelFormat* Format = image->format;
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            put_pixel(image,j,i,blackAndwhite(get_pixel(image,j,i), Format, value));
        }
    }
    SDL_UnlockSurface(image);
}

// Blackscale function
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format, int value)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel, Format, &r, &g, &b);
    if ((r + g + b) / 3 > value)
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
    else
    {
        return SDL_MapRGB(Format, 0, 0, 0);
    }
}

void insertionSort(Uint8 arr[], int n)
{
    int i, j;
    Uint8 key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

Uint32 MedianValue(SDL_Surface *image, int x, int y)
{
    Uint32 pix[9];
    Uint8 tab[9];
    Uint8 r;
    Uint8 g;
    Uint8 b;

    pix[0] = get_pixel(image, x-1, y+1);
    pix[1] = get_pixel(image, x, y+1);
    pix[2] = get_pixel(image, x+1, y+1);
    pix[3] = get_pixel(image, x-1, y);
    pix[4] = get_pixel(image, x, y);
    pix[5] = get_pixel(image, x+1, y);
    pix[6] = get_pixel(image, x-1, y-1);
    pix[7] = get_pixel(image, x, y-1);
    pix[8] = get_pixel(image, x+1, y-1);

    for (int i = 0; i < 9; i++)
    {
        SDL_GetRGB(pix[i], image->format, &r,&g,&b);
        tab[i] = r;
    }
    
    insertionSort(tab, 9);
    int mid = 0;
    for (int j = 2; j <= 6; j++)
        mid += (int) tab[j];
    mid = mid/5;
    return SDL_MapRGB(image->format, tab[4],tab[4],tab[4]);
    //return SDL_MapRGB(image->format, mid,mid,mid);
}

void MedianFilter(SDL_Surface* image){
    for (int y = 1; y < image->h-1; y++)
    {
        for (int x = 1; x < image->w-1; x++)
            put_pixel(image, x, y, MedianValue(image, x, y));
    }
}


void Gamma(SDL_Surface* image_surface){
    Uint32 pixel;
    Uint8 r, g, b;
    Uint8 averager, averageg, averageb;;

    for(int i=0; i<image_surface->h; i++){
        for(int j=0; j<image_surface->w; j++){
            pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            averager = 255*pow((r/255.),.1);
            averageg = 255*pow((g/255.),.1);
            averageb = 255*pow((b/255.),.1);
            pixel = SDL_MapRGB(image_surface->format, averager, averageg, averageb);
            put_pixel(image_surface, j, i, pixel);
        }
    }
}




void grayscale(SDL_Surface* image_surface){
    Uint32 pixel;
    Uint8 r, g, b;
    Uint8 average;

    for(int i=0; i<image_surface->h; i++){
        for(int j=0; j<image_surface->w; j++){
            pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            average = 0.3*r + 0.59 *g + 0.11*b;
            pixel = SDL_MapRGB(image_surface->format, average, average, average);
            put_pixel(image_surface, j, i, pixel);
        }
    }
}




void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

int str_bool(char* arg)
{
    if (arg[1] != '\0')
        errx(2, "Not 1 or 0");
    if (arg[0] == '0')
        return 0;
    return 1;
}

int main( int argc, char* args[] )
{
    if (argc!=4)
    {
        errx(1, "The number of argument(s) is wrong! Please make sure that there is only one argument\n");
    }

    int value=0;
    char *p=args[3];

    while(*p!='\0'){
        value*=10;
        value+=(int)(*p-'0');
        p++;
    }
    printf("%d", value);

    //The surface displayed on the window
    SDL_Surface * screenSurface = NULL;

    //The surface of the image we are gonna load
    SDL_Surface * Loaded = NULL;

    //Initialize SDL video module
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Load image using SDL_image
        Loaded=IMG_Load(args[1]);
        if(!Loaded) 
        {
            printf("IMG_Load: %s\n", IMG_GetError());
        }
        else
        {
            //Create a window that is the same size as our image
            screenSurface = SDL_SetVideoMode( Loaded->w, Loaded->h, 32, SDL_SWSURFACE );
            if(!screenSurface)
            {
                printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Blit the Loaded image over the window's surface
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                //Update the surface
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                //Treat the loaded image
                //
                //Grayscale
                grayscale(Loaded);
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                if (str_bool(args[2]))
                {
                    //Median filter                                                 
                    MedianFilter(Loaded);
                    SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                    SDL_Flip(screenSurface);
                    printf("Filter should have been done");
                    wait_for_keypressed();
                }

                //Gamma
                Gamma(Loaded);
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                //Black and White
                colorTreatment(Loaded, value);
                SDL_SaveBMP(Loaded, "BlackAndWhite.bmp");
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                if (str_bool(args[2]))
                {
                    //Median filter
                    MedianFilter(Loaded);
                    SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                    SDL_Flip(screenSurface);
                    printf("Filter should have been done\n");
                    wait_for_keypressed();   
                }

                //Rotation and update
                Loaded = rotozoomSurface(screenSurface, -90, 1, 1);
                screenSurface = SDL_SetVideoMode( Loaded->w, Loaded->h, 32,SDL_SWSURFACE);
                SDL_BlitSurface(Loaded,NULL,screenSurface,NULL);
                SDL_Flip(screenSurface);

                //Wait for a key to be pressed to end the program
                wait_for_keypressed();
            }
        }
    }
    SDL_FreeSurface( screenSurface );
    SDL_FreeSurface( Loaded );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}





