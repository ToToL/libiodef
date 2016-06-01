#include <libiodef/iodef.h>
#include <string.h>


int main(){
    FILE *file;

    file = fopen("/tmp/example.iodef", "a");
    libiodef_io_t *pio;
    int ret;

    libiodef_io_new(&pio);
    libiodef_io_set_file_io(pio, file);

    iodef_document_t *iodef;
    iodef_incident_t *incident;
    iodef_incident_id_t *incident_id;
    libiodef_string_t *str;

    ret = iodef_document_new(&iodef);
    if ( ret < 0 ) {
            libiodef_perror(ret, "unable to create IODEF message");
            return -1;
    }

    ret = iodef_document_new_incident(iodef, &incident, 0);
    if ( ret < 0 ) {
            libiodef_perror(ret, "unable to create IODEF alert");
            iodef_document_destroy(iodef);
            return -1;
    }

    ret = iodef_incident_new_incident_id(incident, &incident_id);
    if ( ret < 0 ) {
            libiodef_perror(ret, "unable to create IODEF alert");
            iodef_document_destroy(iodef);
            return -1;
    }

    ret = iodef_incident_id_new_name(incident_id, &str);
    if ( ret < 0 ) {
            libiodef_perror(ret, "unable to create IODEF alert");
            iodef_document_destroy(iodef);
            return -1;
    }
    libiodef_string_set_constant(str, "Incident ID");

    iodef_document_set_string(iodef, "incident(0).incident_id.name", "Incident ID");
    iodef_document_set_string(iodef, "incident(1).incident_id.name", "Another Incident ID");

    iodef_time_t *time;
    iodef_time_new_from_gettimeofday(&time);

    iodef_incident_set_report_time(incident, time);

    iodef_document_print(iodef, pio);
    iodef_document_write(iodef, pio);
}
