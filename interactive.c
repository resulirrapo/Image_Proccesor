#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "bmp.h"

#define Path_Size 100
#define load "l"
#define save "s"
#define applyhorizontalflip "ah"
#define applyrotate "ar"
#define applycrop "ac"
#define applyextend "ae"
#define applypaste "ap"
#define createfilter "cf"
#define applyfilter "af"
#define deletfilter "df"
#define deleteimage "di"
#define exit "e"
#define max 100

typedef struct {
    int N, M;
    int ***image;
    int index;
} Image;

typedef struct {
    int N, M, x, y, h, w, rows, cols, new_R, new_G, new_B, index_dst, index_src;
} Sizes;

typedef struct {
    int filter_size;
    float **filter;
} Filter;

// creates and loads the image
int ***loadCommand(int N, int M, char *path) {
    int ***image = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        (image)[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            (image)[i][j] = (int *)malloc(3 * sizeof(int));  // Assuming RGB image
        }
    }
    read_from_bmp(image, N, M, path);
    return image;
}
//  saves the image
void saveCommand(int ****image_list, int index, char *path, Sizes *dimensions) {
    write_to_bmp(image_list[index], dimensions[index].N, dimensions[index].M, path);
}
//  applies the horizontal flip
int ***applyHorizontalFlip(int index, int N, int M, int ****image_list) {
    return flip_horizontal(image_list[index], N, M);
}
//  applies the rotate
int ***applyRotate(int index, int N, int M, int ****image_list, Sizes *dimensions) {
    int temp = dimensions[index].N;
    dimensions[index].N = dimensions[index].M;
    dimensions[index].M = temp;
    return rotate_left(image_list[index], N, M);
}
//  applies the crop
int ***applyCrop(int index, int ****image_list, Sizes *dimensions, int x, int y, int w, int h) {
    // Get N and M from the dimensions array
    int N = dimensions[index].N;
    int M = dimensions[index].M;

    int ***cropped_image = crop(image_list[index], N, M, x, y, h, w);

    // Update dimensions after cropping
    if (cropped_image != NULL) {
        dimensions[index].N = h;
        dimensions[index].M = w;
    }

    return cropped_image;
}
// applies the extend operation
int ***applyExtend(int index, int ****image_list, Sizes *dimensions,
 int rows, int cols, int new_R, int new_G, int new_B) {
    int N = dimensions[index].N;
    int M = dimensions[index].M;

    int ***extended_image = extend(image_list[index], N, M, rows, cols, new_R, new_G, new_B);

        // Update dimensions after extending
        dimensions[index].N = N + (2 * rows);
        dimensions[index].M = M + (2 * cols);

    return extended_image;
}

//  adds image to a list o images
void addImageToList(int ****image_list, int *image_count, int *capacity, int ***new_image, int N, int M) {
    if (*image_count >= *capacity) {
        *capacity *= 2;
        int ****temp = realloc(*image_list, (*capacity) * sizeof(int ***));
        if (temp == NULL) {
            return;
        }
        image_list = temp;
    }

    (image_list)[*image_count] = new_image;
    (image_count)++;
}

int main() {
    char command[3];
    int running = 1;
    char path[Path_Size];
    int index = 0;
    int ***new_image = NULL;
    int N = 0, M = 0;
    int image_count = 0;
    int capacity = max;
    int x = 0, y = 0, w = 0, h = 0;
    int rows = 0, cols = 0, new_R = 0, new_G = 0, new_B = 0;

        // creates a list of images
    int ****image_list = malloc(capacity * sizeof(int ***));
    // dimensions of the images
    Sizes* dimensions = malloc(capacity * sizeof(Sizes));

    while (running) {
        // takes the command and compare it with the list of commands
        scanf("%s", command);
        if (strcmp(command, "e") == 0) {
            running = 0;
        } else if (strcmp(command, "l") == 0) {
            scanf("%d %d %s", &N, &M, path);
            dimensions[index].N = N;
            dimensions[index].M = M;
            new_image = loadCommand(N, M, path);
            addImageToList(image_list, &image_count, &capacity, new_image, N, M);
        } else if (strcmp(command, "s") == 0) {
            scanf("%d %s", &index, path);
            saveCommand(image_list, index, path, dimensions);
        } else if (strcmp(command, "ah") == 0) {
            new_image = applyHorizontalFlip(index, N, M, image_list);
            addImageToList(image_list, &image_count, &capacity, new_image, N, M);
        } else if (strcmp(command, "ar") == 0) {
            new_image = applyRotate(index, dimensions[index].N, dimensions[index].M, image_list, dimensions);
            addImageToList(image_list, &image_count, &capacity, new_image, dimensions[index].N, dimensions[index].M);
        } else if (strcmp(command, "ac") == 0) {
            scanf("%d %d %d %d %d", &index, &x, &y, &w, &h);
            new_image = applyCrop(index, image_list, dimensions, x, y, w, h);
            addImageToList(image_list, &image_count, &capacity, new_image, dimensions[index].N, dimensions[index].M);
        } else if (strcmp(command, "ae") == 0) {
            scanf("%d %d %d %d %d %d %d %d", &index, &rows, &cols, &new_R, &new_G, &new_B, &N, &M);
            new_image = applyExtend(index, image_list, dimensions, rows, cols, new_R, new_G, new_B);
            addImageToList(image_list, &image_count, &capacity, new_image, dimensions[index].N, dimensions[index].M);
        }
    }
    return 0;
}
