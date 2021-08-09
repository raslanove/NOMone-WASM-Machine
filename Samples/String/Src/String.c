
#include <NString.h>
#include <NSystemUtils.h>

void NMain(void) {
    struct NString* string = NString.create("besm Allah %s %s", "AlRa7maan AlRa7eem", ":)");
    NSystemUtils.logE("String Test", "%s", NString.get(string));
    NString.destroyAndFree(string);
}
