
#include <NString.h>
#include <NError.h>
#include <NSystemUtils.h>

static struct NString* initialize(struct NString* outputString) {
    NByteVector.create(4, &outputString->string);
    NByteVector.pushBack(&outputString->string, 0);
    return outputString;
}

static void destroy(struct NString* string) {
    NByteVector.destroy(&(string->string));
}

static void destroyAndFree(struct NString* string) {
    NByteVector.destroy(&(string->string));
    NSystemUtils.free(string);
}

static inline int32_t positiveIntegerToDigits(char* outDigits, uint32_t integer) {
    int32_t digitsCount=0;
    do {
        outDigits[digitsCount++] = integer % 10;
        integer /= 10;
    } while (integer);
    return digitsCount;
}

static inline int32_t positiveLongToDigits(char* outDigits, uint64_t longInteger) {
    int32_t digitsCount=0;
    do {
        outDigits[digitsCount++] = longInteger % 10;
        longInteger /= 10;
    } while (longInteger);
    return digitsCount;
}

/////////////////////////////////////////////////////////////////////////////////////
// Floating point to string conversion
/////////////////////////////////////////////////////////////////////////////////////

// Feel free to modify these 3 numbers:
#define FLOAT_TO_STRING_MAX_INTEGER_DIGITS  19  // Up to 19.
#define FLOAT_TO_STRING_MAX_FRACTION_DIGITS  7  // Up to 18.
#define MIN_FLOAT_NORMALIZATION_EXPONENT     5  // MUST be less than MAX_FRACTION_DIGITS.

struct FloatToStringConversionSettings {
    int32_t  maxIntegerDigitsCount;    // Up to 19.
    int32_t maxFractionDigitsCount;    // Up to 18. Not 19, since one digit will be used for rounding.

    // Precomputed Values provided to avoid having to compute them at runtime.
    float lowerNormalizationThreshold; // 1e(X), where X must be <   maxFractionDigitsCount.
    float upperNormalizationThreshold; // 1e(X), where X must be =    maxIntegerDigitsCount.
    uint64_t fractionDigitsShift;      // 1e(X), where X must be = 1+maxFractionDigitsCount.
};

// Because the arguments to a macro aren't expanded if they appear along with a # or ## in the macro body,
// see: https://stackoverflow.com/a/3221914/1942069
// and: https://stackoverflow.com/q/53803996/1942069
// To test your macros, you can use them in a simple program and use:
// => gcc -C -E SimpleProgram.c
// to see how they end up.
#define POSITIVE_EXPONENT(exp)               1e##exp
#define MACRO_EXPAND_POSITIVE(macro) POSITIVE_EXPONENT(macro)
#define NEGATIVE_EXPONENT(exp)               1e-##exp
#define MACRO_EXPAND_NEGATIVE(macro) NEGATIVE_EXPONENT(macro)

#define MAX_NO_NORMALIZATION_VALUE MACRO_EXPAND_POSITIVE(FLOAT_TO_STRING_MAX_INTEGER_DIGITS)
#define MIN_NO_NORMALIZATION_VALUE MACRO_EXPAND_NEGATIVE(MIN_FLOAT_NORMALIZATION_EXPONENT)
#define FRACTION_SHIFT_VALUE MACRO_EXPAND_POSITIVE(FLOAT_TO_STRING_MAX_FRACTION_DIGITS)

static const struct FloatToStringConversionSettings DEFAULT_DOUBLE_CONVERSION_SETTINGS = {
    .maxIntegerDigitsCount = FLOAT_TO_STRING_MAX_INTEGER_DIGITS,
    .maxFractionDigitsCount = FLOAT_TO_STRING_MAX_FRACTION_DIGITS,
    .upperNormalizationThreshold = MAX_NO_NORMALIZATION_VALUE,
    .lowerNormalizationThreshold = MIN_NO_NORMALIZATION_VALUE,
    .fractionDigitsShift = 10*FRACTION_SHIFT_VALUE
};

// True if a special value,
static inline boolean appendDoubleToByteVector_HandleSignAndSpecialValues(struct NByteVector *outVector, double* value) {

    // Handle NaN (not a number),
    if (NSystemUtils.isNaN(*value)) {

        // Not handling the different types of NAN here. See: https://stackoverflow.com/questions/3596622/negative-nan-is-not-a-nan#comment3775326_3596622
        NByteVector.pushBack(outVector, 'n');
        NByteVector.pushBack(outVector, 'a');
        NByteVector.pushBack(outVector, 'n');
        return True;
    }

    // Make positive,
    if (*value<0) {
        NByteVector.pushBack(outVector, '-');
        *value = 0 - *value;
    }

    // Handle infinity,
    if (NSystemUtils.isInf(*value)) {
        NByteVector.pushBack(outVector, 'i');
        NByteVector.pushBack(outVector, 'n');
        NByteVector.pushBack(outVector, 'f');
        return True;
    }

    return False;
}

static inline void appendFloatToByteVector_Normalize(float* in_out_Value, int32_t* outExponent, const struct FloatToStringConversionSettings* conversionSettings) {

    // Normalize. Turn extremely large/small values into exponent format, like following:
    //  => Assume value is 1234.000. Normalizing it would result in 1.234e3.
    //  => Assume value is 0.001234. Normalizing it would result in 1.234e-3.
    // Note that (1e3 = 1e1*1e2), (1e5 = 1e4*1e1) and (1e7= 1e4*1e2*1e1), like binary numbers.
    // Also note, float limits are: 1.175494351e-38 to 3.402823466e38.
    float value = *in_out_Value;
    int32_t exponent=0;
    if (value >= conversionSettings->upperNormalizationThreshold) {
        if (value >= 1e32 ) { exponent +=32; value *= 1e-32; }
        if (value >= 1e16 ) { exponent +=16; value *= 1e-16; }
        if (value >= 1e8  ) { exponent += 8; value *= 1e-8 ; }
        if (value >= 1e4  ) { exponent += 4; value *= 1e-4 ; }
        if (value >= 1e2  ) { exponent += 2; value *= 1e-2 ; }
        if (value >= 1e1  ) { exponent += 1; value *= 1e-1 ; }
    } else if (value <= conversionSettings->lowerNormalizationThreshold) {
        if (value <  1e-31) { exponent -=32; value *= 1e32 ; }
        if (value <  1e-15) { exponent -=16; value *= 1e16 ; }
        if (value <  1e-7 ) { exponent -= 8; value *= 1e8  ; }
        if (value <  1e-3 ) { exponent -= 4; value *= 1e4  ; }
        if (value <  1e-1 ) { exponent -= 2; value *= 1e2  ; }
        if (value <  1    ) { exponent -= 1; value *= 1e1  ; }
    }

    *in_out_Value = value;
    *outExponent = exponent;
}

static inline void appendDoubleToByteVector_Normalize(double* in_out_Value, int32_t* outExponent, const struct FloatToStringConversionSettings* conversionSettings) {

    // Normalize. Turn extremely large/small values into exponent format, like following:
    //  => Assume value is 1234.000. Normalizing it would result in 1.234e3.
    //  => Assume value is 0.001234. Normalizing it would result in 1.234e-3.
    // Note that (1e3 = 1e1*1e2), (1e5 = 1e4*1e1) and (1e7= 1e4*1e2*1e1), like binary numbers.
    // Also note, double limits are: 2.2250738585072014e-308 to 1.7976931348623158e308.
    double value = *in_out_Value;
    int32_t exponent=0;
    if (value >= conversionSettings->upperNormalizationThreshold) {
        if (value >= 1e256 ) { exponent +=256; value *= 1e-256; }
        if (value >= 1e128 ) { exponent +=128; value *= 1e-128; }
        if (value >= 1e64  ) { exponent += 64; value *= 1e-64 ; }
        if (value >= 1e32  ) { exponent += 32; value *= 1e-32 ; }
        if (value >= 1e16  ) { exponent += 16; value *= 1e-16 ; }
        if (value >= 1e8   ) { exponent +=  8; value *= 1e-8  ; }
        if (value >= 1e4   ) { exponent +=  4; value *= 1e-4  ; }
        if (value >= 1e2   ) { exponent +=  2; value *= 1e-2  ; }
        if (value >= 1e1   ) { exponent +=  1; value *= 1e-1  ; }
    } else if (value <= conversionSettings->lowerNormalizationThreshold) {
        if (value <  1e-256) { exponent -=256; value *= 1e256 ; }
        if (value <  1e-128) { exponent -=128; value *= 1e128 ; }
        if (value <  1e-64 ) { exponent -= 64; value *= 1e64  ; }
        if (value <  1e-31 ) { exponent -= 32; value *= 1e32  ; }
        if (value <  1e-15 ) { exponent -= 16; value *= 1e16  ; }
        if (value <  1e-7  ) { exponent -=  8; value *= 1e8   ; }
        if (value <  1e-3  ) { exponent -=  4; value *= 1e4   ; }
        if (value <  1e-1  ) { exponent -=  2; value *= 1e2   ; }
        if (value <  1     ) { exponent -=  1; value *= 1e1   ; }
    }

    *in_out_Value = value;
    *outExponent = exponent;
}

static inline void appendDoubleToByteVector_HandleTextFormatting(struct NByteVector *outVector, double value, int32_t exponent, const struct FloatToStringConversionSettings* conversionSettings) {

    // Extract the integer and fraction parts as is, whether they were normalized or not,
    uint64_t integerPart = (uint64_t) value;
    value -= integerPart;
    uint64_t fractionPart = (uint64_t) (value * conversionSettings->fractionDigitsShift);

    // Rounding,
    int32_t lastFractionDigit = fractionPart%10;
    if (lastFractionDigit > 4) {
        fractionPart += 10 - lastFractionDigit;
        if (fractionPart == conversionSettings->fractionDigitsShift) {
            fractionPart = 0;
            integerPart++;
        }
    } else {
        fractionPart -= lastFractionDigit;
    }

    // Convert the integer part to string,
    {
        char integerDigits[conversionSettings->maxIntegerDigitsCount];
        int32_t digitsCount = positiveLongToDigits(integerDigits, integerPart);
        while (digitsCount--) NByteVector.pushBack(outVector, integerDigits[digitsCount]+48);
    }

    // Convert the fraction part, whether normalized or not,
    if (fractionPart) {
        NByteVector.pushBack(outVector, '.');
        int32_t digitsCount = 1+conversionSettings->maxFractionDigitsCount; // 1 extra digit is used for rounding.
        char fractionDigits[digitsCount];
        for (int i=digitsCount-1; i; i--) fractionDigits[i] = 0; // Index 0 needn't be set, it should be overwritten in all cases.
        positiveLongToDigits(fractionDigits, fractionPart);

        // Remove trailing zeroes and push characters,
        int32_t removedZeroesCount=0;
        while (!fractionDigits[removedZeroesCount]) removedZeroesCount++;
        digitsCount -= removedZeroesCount;
        while (digitsCount--) NByteVector.pushBack(outVector, fractionDigits[removedZeroesCount+digitsCount]+48);
    }

    // Finally, append the exponent,
    if (exponent) {
        NByteVector.pushBack(outVector, 'e');
        if (exponent < 0) {
            NByteVector.pushBack(outVector, '-');
            exponent = 0 - exponent;
        }
        char exponentDigits[3];
        int32_t digitsCount=positiveLongToDigits(exponentDigits, exponent);
        while (digitsCount--) NByteVector.pushBack(outVector, exponentDigits[digitsCount]+48);
    }
}

static void appendFloatToByteVector(struct NByteVector *outVector, float floatValue, const struct FloatToStringConversionSettings* conversionSettings) {

    // Following this algorithm: https://blog.benoitblanchon.fr/lightweight-float-to-string/

    // Handle sign, NaN and Inf,
    double doubleValue = floatValue;
    if (appendDoubleToByteVector_HandleSignAndSpecialValues(outVector, &doubleValue)) return ;
    floatValue = doubleValue;

    // Normalize (if needed),
    int32_t exponent;
    appendFloatToByteVector_Normalize(&floatValue, &exponent, conversionSettings);

    // Format and append,
    appendDoubleToByteVector_HandleTextFormatting(outVector, floatValue, exponent, conversionSettings);
}

static void appendDoubleToByteVector(struct NByteVector *outVector, double doubleValue, const struct FloatToStringConversionSettings* conversionSettings) {

    // Following this algorithm: https://blog.benoitblanchon.fr/lightweight-float-to-string/

    // Handle sign, NaN and Inf,
    if (appendDoubleToByteVector_HandleSignAndSpecialValues(outVector, &doubleValue)) return ;

    // Normalize (if needed),
    int32_t exponent;
    appendDoubleToByteVector_Normalize(&doubleValue, &exponent, conversionSettings);

    // Format and append,
    appendDoubleToByteVector_HandleTextFormatting(outVector, doubleValue, exponent, conversionSettings);
}

/////////////////////////////////////////////////////////////////////////////////////
// Generating text from format
/////////////////////////////////////////////////////////////////////////////////////

static struct NString* vAppend(struct NString* outString, const char* format, va_list vaList) {

    // Pop the termination zero,
    struct NByteVector *outVector = &outString->string;
    char tempChar;
    NByteVector.popBack(outVector, &tempChar);

    // Parse the format string,
    int32_t index=0;
    char currentChar;
    while (currentChar = format[index++]) {

        // Regular characters,
        if (currentChar!='%') {
            NByteVector.pushBack(outVector, currentChar);
            continue;
        }

        // Found a '%',
        currentChar = format[index++];
        switch(currentChar) {
            case '%': {
                NByteVector.pushBack(outVector, '%');
                continue;
            }
            case 's': {
                const char* sourceString = va_arg(vaList, const char*);

                int32_t stringIndex = 0;
                char currentChar;
                while (currentChar = sourceString[stringIndex++]) {
                    NByteVector.pushBack(outVector, currentChar);
                }
                continue;
            }
            case 'c': {
                char sourceChar = (char) va_arg(vaList, int); // ‘char’ is promoted to ‘int’ when passed through ‘...’.
                NByteVector.pushBack(outVector, sourceChar);
                continue;
            }
            case 'd': {
                int32_t sourceInteger = (int32_t) va_arg(vaList, int32_t);

                if (sourceInteger < 0) {
                    NByteVector.pushBack(outVector, '-');
                    sourceInteger = 0-sourceInteger;
                }

                char digits[10];
                int32_t digitsCount = positiveIntegerToDigits(digits, sourceInteger);

                // Push digits in reverse order,
                while (digitsCount--) NByteVector.pushBack(outVector, digits[digitsCount]+48);

                continue;
            }
            case 'l': {

                // Check if a 'd' follows,
                currentChar = format[index++];
                if (currentChar != 'd') {
                    NERROR("NString.vAppend", "Expected \"%sd%s\" after \"%s%%l%s\" in format string: %s%s", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), format);
                    goto exit;
                }

                int64_t sourceLong = (int64_t) va_arg(vaList, int64_t);

                if (sourceLong < 0) {
                    NByteVector.pushBack(outVector, '-');
                    sourceLong = 0 - sourceLong;
                }

                char digits[19];
                int32_t digitsCount=positiveLongToDigits(digits, sourceLong);

                // Push digits in reverse order,
                while (digitsCount--) NByteVector.pushBack(outVector, digits[digitsCount]+48);

                continue;
            }
            case 'f': {
                double sourceDouble = (double) va_arg(vaList, double);
                appendDoubleToByteVector(outVector, sourceDouble, &DEFAULT_DOUBLE_CONVERSION_SETTINGS);
                continue;
            }

            default:
                NERROR("NString.vAppend", "Unexpected sequence: \"%s%%%c%s\" in format string: %s%s", NTCOLOR(HIGHLIGHT), currentChar, NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), format);
                goto exit;
        }
    }

    // Add the termination zero,
    exit:
    NByteVector.pushBack(outVector, 0);

    return outString;
}

static struct NString* append(struct NString* outString, const char* format, ...) {
    va_list vaList;
    va_start(vaList, format);
    vAppend(outString, format, vaList);
    va_end(vaList);
    return outString;
}

static struct NString* set(struct NString* outString, const char* format, ...) {
    NByteVector.clear(&(outString->string));
    NByteVector.pushBack(&outString->string, 0);

    va_list vaList;
    va_start(vaList, format);
    vAppend(outString, format, vaList);
    va_end(vaList);
    return outString;
}

static const char* get(struct NString* string) {
    return string->string.objects;
}

static struct NString* create(const char* format, ...) {
    struct NString* outString = NSystemUtils.malloc(sizeof(struct NString));
    initialize(outString);

    va_list vaList;
    va_start(vaList, format);
    vAppend(outString, format, vaList);
    va_end(vaList);
    return outString;
}

static struct NString* replace(const char* textToBeSearched, const char* textToBeRemoved, const char* textToBeInserted) {

    // Create a new string for the result,
    struct NString* newString = NString.create("");

    int32_t searchIndex=0;
    char currentChar;
    while (currentChar = textToBeSearched[searchIndex]) {

        // Attempt matching,
        int32_t matchIndex=0;
        while (textToBeRemoved[matchIndex] && textToBeSearched[searchIndex+matchIndex] &&
               textToBeRemoved[matchIndex] == textToBeSearched[searchIndex+matchIndex]) {
            matchIndex++;
        }

        // Check if a match occurred,
        if (!textToBeRemoved[matchIndex]) {
            NString.append(newString, "%s", textToBeInserted);
            searchIndex += matchIndex;
        } else {
            NString.append(newString, "%c", currentChar);
            searchIndex++;
        }
    }

    return newString;
}

static int32_t length(struct NString* string) {
    return string->string.size - 1;
}

const struct NString_Interface NString = {
    .initialize = initialize,
    .destroy = destroy,
    .destroyAndFree = destroyAndFree,
    .vAppend = vAppend,
    .append = append,
    .set = set,
    .get = get,
    .create = create,
    .replace = replace,
    .length = length
};
