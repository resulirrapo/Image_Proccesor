#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#define a 0
#define b 255

// TODO(Rrapo) Task 1
int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M / 2; j++) {
            int *temp = image[i][j];
            image[i][j] = image[i][M - 1 - j];
            image[i][M - 1 - j] = temp;
        }
    }
    return image;
}

// TODO(Rrapo) Task 2
int ***rotate_left(int ***image, int N, int M) {
    // Allocate memory for the rotated image
    int ***new_image = (int ***) malloc(M * sizeof(int **));
    for (int i = 0; i < M; i++) {
        new_image[i] = (int **) malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++) {
            new_image[i][j] = (int * ) malloc(sizeof(int));
        }
    }

    // Rotate the image to the left
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            new_image[M - 1 - j][i][0] = image[i][j][0];
            new_image[M - 1 - j][i][1] = image[i][j][1];
            new_image[M - 1 - j][i][2] = image[i][j][2];
        }
    }
    // free the original matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return new_image;
}

// TODO(Rrapo) Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    // allocate memory for the new matrix
    int ***croped_image = (int ***) malloc(h * sizeof(int **));
    for (int i = 0; i < h; i++) {
        croped_image[i] = (int **) malloc(w * sizeof(int *));
        for (int j = 0; j < w; j++) {
            croped_image[i][j] = (int *) malloc(3 * sizeof(int));
        }
    }

    // adding the new coordinates to the croped matrix
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            croped_image[i][j][0] = image[y + i][x + j][0];
            croped_image[i][j][1] = image[y + i][x + j][1];
            croped_image[i][j][2] = image[y + i][x + j][2];
        }
    }


    // free the original matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
        free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);


    return croped_image;
}

// TODO(Rrapo) Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    // adding lines and columns
    int height = N + (2 * rows);
    int width = M + (2 * cols);
    // allocate memory for the new matrix
    int ***extended_image = (int ***) malloc(height * sizeof(int **));
    for (int i = 0; i < height; i++)  {
        extended_image[i] = (int **) malloc(width * sizeof(int *));
        for (int j = 0; j < width; j++) {
            extended_image[i][j] = (int *) malloc(sizeof(int));
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // checking if the current pixels are above || below || on the left
            // || on the right of the added rows || columns
            if (i < rows || i >= N + rows || j < cols || j >= M + cols) {
                        extended_image[i][j][0] = new_R;
                        extended_image[i][j][1] = new_G;
                        extended_image[i][j][2] = new_B;
                 } else {
                    // croping the image to its new position if the pixels didn change borders
                        extended_image[i][j][0] = image[i - rows][j - cols][0];
                        extended_image[i][j][1] = image[i - rows][j - cols][1];
                        extended_image[i][j][2] = image[i - rows][j - cols][2];
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return extended_image;
}

// TODO(Rrapo) Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int ***image_src, int N_src, int M_src, int x, int y) {
    for (int i = 0 ; i < N_src; i++) {
        for (int j = 0; j < M_src; j++) {
            // add the coordinates to the destination
            int i_dst = y + i;
            int j_dst = x + j;
            // checking if the image is out of bound
            if (i_dst < N_dst && j_dst < M_dst) {
                image_dst[i_dst][j_dst][0] = image_src[i][j][0];
                image_dst[i_dst][j_dst][1] = image_src[i][j][1];
                image_dst[i_dst][j_dst][2] = image_src[i][j][2];
        }
    }
}
return image_dst;
}

// TODO(Rrapo) Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    // allocate the filtered image
    int ***filtered_image = (int ***) malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        filtered_image[i] = (int **) malloc(M * sizeof(int*));
        for (int j = 0; j < M; j++) {
            filtered_image[i][j] = (int *) malloc(sizeof(int));
        }
    }
    // taking the center of matrix as a reference
    int middle = filter_size / 2;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            float R = 0, G = 0, B = 0;
                // starting from the the neighbour lines and columns adn adding the filter
            for (int fi = -middle; fi <= middle; fi++) {
                for (int fj = -middle; fj <= middle; fj++) {
                    int i_dst = i + fi;
                    int j_dst = j + fj;

                        // checking if we are not out of boudaries
                    if (i_dst >= 0 && i_dst < N && j_dst >= 0 && j_dst < M) {
                        R = R + (float)image[i_dst][j_dst][0] * filter[fi + middle][fj + middle];
                        G = G + (float)image[i_dst][j_dst][1] * filter[fi + middle][fj + middle];
                        B = B + (float)image[i_dst][j_dst][2] * filter[fi + middle][fj + middle];
                    }
                }
            }
            R = (R < a) ? a : (R > b) ? b : R;
            G = (G < a) ? a : (G > b) ? b : G;
            B = (B < a) ? a : (B > b) ? b : B;

            filtered_image[i][j][0] = (int)R;
            filtered_image[i][j][1] = (int)G;
            filtered_image[i][j][2] = (int)B;
        }
    }
    return filtered_image;
}
