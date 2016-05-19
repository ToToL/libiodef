#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "prelude.h"


static void set_value_check(iodef_document_t *iodef, const char *paths,
                            const char *str_value, prelude_bool_t verify_get)
{
        iodef_path_t *path;
        iodef_value_t *value;
        prelude_string_t *str;
        prelude_string_t *res;

        assert(iodef_path_new_fast(&path, paths) == 0);
        assert(prelude_string_new_ref(&str, str_value) == 0);
        assert(iodef_value_new_string(&value, str) == 0);

        if ( verify_get )
                assert(iodef_path_set(path, iodef, value) == 0);
        else
                assert(iodef_path_set(path, iodef, value) < 0);

        iodef_value_destroy(value);

        if ( ! verify_get ) {
                iodef_path_destroy(path);
                return;
        }

        assert(iodef_path_get(path, iodef, &value) > 0);

        assert(prelude_string_new(&res) == 0);
        assert(iodef_value_to_string(value, res) >= 0);
        assert(strcmp(str_value, prelude_string_get_string(res)) == 0);
        prelude_string_destroy(res);

        iodef_value_destroy(value);
        iodef_path_destroy(path);
}

int main(void)
{
        int i, ret;
        iodef_value_t *value;
        iodef_path_t *path;
        iodef_document_t *iodef;
        struct {
                const char *path;
                int depth;
                prelude_bool_t has_list;
                prelude_bool_t ambiguous;
                prelude_bool_t successful;
        } plist[] = {
                { "alert.classification.text", 3, FALSE, FALSE, TRUE },
                { "alert.assessment.impact.severity", 4, FALSE, FALSE, TRUE },
                { "alert.source.node.name", 4, TRUE, TRUE, TRUE },
                { "alert.target(0).node.name", 4, TRUE, FALSE, TRUE },
                { "alert.invalid.path", 0, FALSE, FALSE, FALSE }
        };

        assert(iodef_document_new(&iodef) == 0);

        for ( i = 0; i < sizeof(plist) / sizeof(*plist); i++ ) {
                ret = iodef_path_new_fast(&path, plist[i].path);
                assert((plist[i].successful == TRUE && ret == 0) || (plist[i].successful == FALSE && ret < 0));

                if ( ret < 0 )
                        continue;

                assert(strcmp(plist[i].path, iodef_path_get_name(path, -1)) == 0);
                assert(iodef_path_get_depth(path) == plist[i].depth);
                assert(iodef_path_has_lists(path) == plist[i].has_list);
                assert(iodef_path_is_ambiguous(path) == plist[i].ambiguous);

                if ( plist[i].ambiguous ) {
                        iodef_path_destroy(path);
                        continue;
                }

                /*
                 * Check whether setting NULL value work.
                 */
                ret = iodef_path_set(path, iodef, NULL);
                assert(ret == 0);

                ret = iodef_path_get(path, iodef, &value);
                assert(ret == 0); /* empty value */

                iodef_path_destroy(path);
        }

        set_value_check(iodef, "alert.classification.text", "Random value", TRUE);
        set_value_check(iodef, "alert.assessment.impact.severity", "high", TRUE);
        set_value_check(iodef, "alert.assessment.impact.severity", "Invalid enumeration", FALSE);

        iodef_document_destroy(iodef);

        exit(0);
}
