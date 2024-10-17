#ifndef UTILS_H_
#define UTILS_H_

static void clear_float_buffer(float *buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        buffer[i] = 0;
    }
}

static void copy_float_buffer(const float *in, float *out, int size)
{
    for (int i = 0; i < size; i++)
    {
        out[i] = in[i];
    }
}

static void reverse_float_buffer(const float *in, float *out, int size)
{
    for (int i = 0; i < size; i++)
    {
        out[i] = in[size - i - 1];
    }
}

static bool compare_float_buffer(const float *a, const float *b, int size,
                                 const float error_threshold, float *max_err)
{
    *max_err = 0.0f;
    for (int i = 0; i < size; i++)
    {
        float err = a[i] - b[i];
        if (err < 0)
        {
            err = -err;
        }
        if (err > *max_err)
        {
            *max_err = err;
        }
    }

    return (*max_err < error_threshold);
}

#endif // UTILS_H_
