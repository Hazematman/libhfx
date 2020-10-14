#include <hfx.h>
#include <hfx_types.h>
#include <string.h>
#include <math.h>

void hfx_matrix_multiply(hfx_state *state, float *a, float *b, float *result)
{
    float temp[16] = {0};

    for(int i=0; i < 4; i++)
    {
        for(int j=0; j < 4; j++)
        {
            for(int k=0; k < 4; k++)
            {
                temp[i*4 + j] += a[k*4 + j]*b[i*4 + k];
            }
        }
    }

    memcpy(result, temp, sizeof(temp));
}

void hfx_matrix_vector_multiply(hfx_state *state, float *mat, float *vec, float *result)
{
    float temp[4] = {0};

    for(int i=0; i < 4; i++)
    {
        for(int j=0; j < 4; j++)
        {
            temp[i] += mat[j*4 + i]*vec[j];
        }
    }

    memcpy(result, temp, sizeof(temp));
}

void hfx_normalize(hfx_state *state, float *vector, float *result)
{
    float temp[4];
    float mag = sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2] + vector[3]*vector[3]);

    temp[0] = vector[0]/mag;
    temp[1] = vector[1]/mag;
    temp[2] = vector[2]/mag;
    temp[3] = vector[3]/mag;
    
    memcpy(result, temp, sizeof(temp));
}

void hfx_load_identity(hfx_state *state)
{
    state->model_matrix[0] = 1.0f;
    state->model_matrix[1] = 0.0f;
    state->model_matrix[2] = 0.0f;
    state->model_matrix[3] = 0.0f;

    state->model_matrix[4] = 0.0f;
    state->model_matrix[5] = 1.0f;
    state->model_matrix[6] = 0.0f;
    state->model_matrix[7] = 0.0f;

    state->model_matrix[8] = 0.0f;
    state->model_matrix[9] = 0.0f;
    state->model_matrix[10] = 1.0f;
    state->model_matrix[11] = 0.0f;

    state->model_matrix[12] = 0.0f;
    state->model_matrix[13] = 0.0f;
    state->model_matrix[14] = 0.0f;
    state->model_matrix[15] = 1.0f;
}

void hfx_translate_f(hfx_state *state, float x, float y, float z)
{
    float result[16] = {0};
    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[12] = x;
    result[13] = y;
    result[14] = z;
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}

void hfx_rotate_f(hfx_state *state, float angle, float x, float y, float z)
{
    float axis[4] = {0};
    float normalized_axis[4];
    float result[16];
    float angle_rad = (angle * M_PI) / 180;
    float c = cos(angle_rad);
    float s = sin(angle_rad);

    axis[0] = x;
    axis[1] = y;
    axis[2] = z;
    
    hfx_normalize(state, axis, normalized_axis);

    result[0] = c + (1-c)*normalized_axis[0]*normalized_axis[0];
    result[1] = (1-c)*normalized_axis[0]*normalized_axis[1] + s*normalized_axis[2];
    result[2] = (1-c)*normalized_axis[0]*normalized_axis[2] - s*normalized_axis[1];
    result[3] = 0.0f;

    result[4] = (1-c)*normalized_axis[1]*normalized_axis[0] - s*normalized_axis[2];
    result[5] = c + (1-c)*normalized_axis[1]*normalized_axis[1];
    result[6] = (1-c)*normalized_axis[1]*normalized_axis[2] + s*normalized_axis[0];
    result[7] = 0.0f;

    result[8] = (1-c)*normalized_axis[2]*normalized_axis[0] + s*normalized_axis[1];
    result[9] = (1-c)*normalized_axis[2]*normalized_axis[1] - s*normalized_axis[0];
    result[10] = c + (1-c)*normalized_axis[2]*normalized_axis[2];
    result[11] = 0.0f;

    result[12] = 0.0f;
    result[13] = 0.0f;
    result[14] = 0.0f;
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}

void hfx_scale_f(hfx_state *state, float sx, float sy, float sz)
{
    float result[16] = {0};

    result[0] = sx;
    result[5] = sy;
    result[10] = sz;
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}