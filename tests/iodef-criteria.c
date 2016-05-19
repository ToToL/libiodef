#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include "prelude.h"

static void test_criteria(iodef_document_t *iodef, const char *criteria_str, int expect_create, int expect_match)
{
        iodef_criteria_t *criteria;

        if ( expect_create < 0 ) {
                assert(iodef_criteria_new_from_string(&criteria, criteria_str) < 0);
                return;
        } else
                assert(iodef_criteria_new_from_string(&criteria, criteria_str) == 0);

        assert(iodef_criteria_match(criteria, iodef) == expect_match);
        iodef_criteria_destroy(criteria);

}

int main(void)
{
        iodef_time_t *ctime;
        iodef_alert_t *alert;
        iodef_document_t *iodef;
        iodef_classification_t *classification;
        prelude_string_t *str;

        assert(prelude_string_new_ref(&str, "A") == 0);

        assert(iodef_document_new(&iodef) == 0);
        assert(iodef_document_new_alert(iodef, &alert) == 0);
        assert(iodef_alert_new_classification(alert, &classification) == 0);
        iodef_classification_set_text(classification, str);

        iodef_document_set_string(iodef, "alert.analyzer(0).name", "A");
        iodef_document_set_string(iodef, "alert.analyzer(1).name", "B");
        iodef_document_set_string(iodef, "alert.analyzer(1).ostype", "C");
        iodef_document_set_string(iodef, "alert.analyzer(1).process.arg(0)", "a0");
        iodef_document_set_string(iodef, "alert.analyzer(1).process.arg(2)", "a2");

        test_criteria(iodef, "alert", 0, 1);
        test_criteria(iodef, "heartbeat", 0, 0);
        test_criteria(iodef, "alert || heartbeat", 0, 1);
        test_criteria(iodef, "alert.classification.txt == A", -1, -1);
        test_criteria(iodef, "alert.classification.text = (A || B || C || D) || heartbeat", 0, 1);
        test_criteria(iodef, "(alert.classification.text == A || heartbeat", -1, -1);

        prelude_string_set_ref(str, "My String");

        test_criteria(iodef, "alert.classification.text != 'My String'", 0, 0);
        test_criteria(iodef, "alert.classification.text != 'random'", 0, 1);

        test_criteria(iodef, "alert.classification.text == 'My String'", 0, 1);
        test_criteria(iodef, "alert.classification.text <> 'My'", 0, 1);
        test_criteria(iodef, "alert.classification.text <> 'my'", 0, 0);
        test_criteria(iodef, "alert.classification.text <>* 'my'", 0, 1);

        test_criteria(iodef, "alert.classification.text ~ 'My String'", 0, 1);
        test_criteria(iodef, "alert.classification.text ~ 'My (String|Check)'", 0, 1);
        test_criteria(iodef, "alert.classification.text ~ 'my'", 0, 0);
        test_criteria(iodef, "alert.classification.text ~* 'my'", 0, 1);

        iodef_alert_new_create_time(alert, &ctime);
        assert(iodef_time_set_from_string(ctime, "2015-05-03 1:59:08") == 0);

        /*
         * Regular time operator check
         */
        test_criteria(iodef, "alert.create_time == '2015-05-03 1:59:08'", 0, 1);
        test_criteria(iodef, "alert.create_time != '2015-05-03 1:59:08'", 0, 0);
        test_criteria(iodef, "alert.create_time < '2015-05-03 1:59:08'", 0, 0);
        test_criteria(iodef, "alert.create_time > '2015-05-03 1:59:08'", 0, 0);
        test_criteria(iodef, "alert.create_time <= '2015-05-03 1:59:08'", 0, 1);
        test_criteria(iodef, "alert.create_time >= '2015-05-03 1:59:08'", 0, 1);

        test_criteria(iodef, "alert.create_time == '2015-05-03 1:59:07'", 0, 0);
        test_criteria(iodef, "alert.create_time != '2015-05-03 1:59:07'", 0, 1);
        test_criteria(iodef, "alert.create_time < '2015-05-03 1:59:07'", 0, 0);
        test_criteria(iodef, "alert.create_time > '2015-05-03 1:59:07'", 0, 1);
        test_criteria(iodef, "alert.create_time <= '2015-05-03 1:59:07'", 0, 0);
        test_criteria(iodef, "alert.create_time >= '2015-05-03 1:59:07'", 0, 1);

        test_criteria(iodef, "alert.create_time < '2015-05-03 1:59:09'", 0, 1);
        test_criteria(iodef, "alert.create_time > '2015-05-03 1:59:09'", 0, 0);
        test_criteria(iodef, "alert.create_time <= '2015-05-03 1:59:09'", 0, 1);
        test_criteria(iodef, "alert.create_time >= '2015-05-03 1:59:09'", 0, 0);

        /*
         * Broken down time check
         */
        assert(iodef_time_set_from_string(ctime, "2015-05-04 00:00:00+00:00") == 0);
        test_criteria(iodef, "alert.create_time == 'month:may mday:3'", 0, 0);
        test_criteria(iodef, "alert.create_time != 'month:may mday:3'", 0, 1);
        test_criteria(iodef, "alert.create_time < 'month:may mday:3'", 0, 0);
        test_criteria(iodef, "alert.create_time > 'month:may mday:3'", 0, 1);
        test_criteria(iodef, "alert.create_time <= 'month:may mday:3'", 0, 0);
        test_criteria(iodef, "alert.create_time >= 'month:may mday:3'", 0, 1);

        test_criteria(iodef, "alert.create_time == 'month:may mday:4'", 0, 1);
        test_criteria(iodef, "alert.create_time != 'month:may mday:4'", 0, 0);
        test_criteria(iodef, "alert.create_time < 'month:may mday:4'", 0, 0);
        test_criteria(iodef, "alert.create_time > 'month:may mday:4'", 0, 0);
        test_criteria(iodef, "alert.create_time <= 'month:may mday:4'", 0, 1);
        test_criteria(iodef, "alert.create_time >= 'month:may mday:4'", 0, 1);

        test_criteria(iodef, "alert.create_time == 'month:may mday:5'", 0, 0);
        test_criteria(iodef, "alert.create_time != 'month:may mday:5'", 0, 1);
        test_criteria(iodef, "alert.create_time < 'month:may mday:5'", 0, 1);
        test_criteria(iodef, "alert.create_time > 'month:may mday:5'", 0, 0);
        test_criteria(iodef, "alert.create_time <= 'month:may mday:5'", 0, 1);
        test_criteria(iodef, "alert.create_time >= 'month:may mday:5'", 0, 0);

        /*
         * Broken down time special wday/yday fields
         */
        test_criteria(iodef, "alert.create_time == 'wday:monday'", 0, 1);
        test_criteria(iodef, "alert.create_time != 'wday:monday'", 0, 0);
        test_criteria(iodef, "alert.create_time == 'wday:tuesday'", 0, 0);
        test_criteria(iodef, "alert.create_time != 'wday:tuesday'", 0, 1);

        test_criteria(iodef, "alert.create_time == 'wday:monday mday:3'", 0, 0);
        test_criteria(iodef, "alert.create_time != 'wday:monday mday:3'", 0, 1);
        test_criteria(iodef, "alert.create_time < 'wday:monday mday:3'", 0, 0);
        test_criteria(iodef, "alert.create_time > 'wday:monday mday:3'", 0, 1);
        test_criteria(iodef, "alert.create_time <= 'wday:monday mday:3'", 0, 0);
        test_criteria(iodef, "alert.create_time >= 'wday:monday mday:3'", 0, 1);

        test_criteria(iodef, "alert.create_time == 'wday:monday mday:4'", 0, 1);
        test_criteria(iodef, "alert.create_time != 'wday:monday mday:4'", 0, 0);
        test_criteria(iodef, "alert.create_time < 'wday:monday mday:4'", 0, 0);
        test_criteria(iodef, "alert.create_time > 'wday:monday mday:4'", 0, 0);
        test_criteria(iodef, "alert.create_time <= 'wday:monday mday:4'", 0, 1);
        test_criteria(iodef, "alert.create_time >= 'wday:monday mday:4'", 0, 1);

        test_criteria(iodef, "alert.create_time == 'wday:monday mday:5'", 0, 0);
        test_criteria(iodef, "alert.create_time != 'wday:monday mday:5'", 0, 1);
        test_criteria(iodef, "alert.create_time < 'wday:monday mday:5'", 0, 1);
        test_criteria(iodef, "alert.create_time > 'wday:monday mday:5'", 0, 0);
        test_criteria(iodef, "alert.create_time <= 'wday:monday mday:5'", 0, 1);
        test_criteria(iodef, "alert.create_time >= 'wday:monday mday:5'", 0, 0);

        /*
         * Test on listed object without specific index
         */
        test_criteria(iodef, "alert.analyzer(*).name == 'A'", 0, 1);
        test_criteria(iodef, "alert.analyzer(*).name != 'A'", 0, 0);
        test_criteria(iodef, "alert.analyzer(*).name == 'NOT EXIST'", 0, 0);
        test_criteria(iodef, "alert.analyzer(*).name != 'NOT EXIST'", 0, 1);
        test_criteria(iodef, "alert.analyzer(*).ostype == 'C'", 0, 1);
        test_criteria(iodef, "alert.analyzer(*).ostype != 'C'", 0, 0);
        test_criteria(iodef, "alert.analyzer(*).ostype == 'NOT EXIST'", 0, 0);
        test_criteria(iodef, "alert.analyzer(*).ostype != 'NOT EXIST'", 0, 1);

        test_criteria(iodef, "alert.analyzer(*).class", 0, 0);
        test_criteria(iodef, "! alert.analyzer(*).class", 0, 1);

        test_criteria(iodef, "alert.analyzer(*).name", 0, 1);
        test_criteria(iodef, "! alert.analyzer(*).name", 0, 0);

        test_criteria(iodef, "alert.analyzer(*).ostype", 0, 1);
        test_criteria(iodef, "! alert.analyzer(*).ostype", 0, 0);

        test_criteria(iodef, "alert.source(*).interface", 0, 0);
        test_criteria(iodef, "! alert.source(*).interface", 0, 1);

        test_criteria(iodef, "alert.source", 0, 0);
        test_criteria(iodef, "! alert.source", 0, 1);

        test_criteria(iodef, "alert.analyzer", 0, 1);
        test_criteria(iodef, "! alert.analyzer", 0, 0);

        test_criteria(iodef, "alert.analyzer(*).process.arg", 0, 1);
        test_criteria(iodef, "! alert.analyzer(*).process.arg", 0, 0);
        test_criteria(iodef, "alert.analyzer(*).process.arg == 'a0'", 0, 1);
        test_criteria(iodef, "alert.analyzer(*).process.arg != 'a0'", 0, 0);
        test_criteria(iodef, "alert.analyzer(*).process.arg == 'NOT EXIST'", 0, 0);
        test_criteria(iodef, "alert.analyzer(*).process.arg != 'NOT EXIST'", 0, 1);

        iodef_document_destroy(iodef);
        exit(0);
}
