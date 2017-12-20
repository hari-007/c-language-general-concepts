/*  Autor: hari-007 (Hari)
 *  Description: Using POSIX unix system calls to do read, open, write & close file functionality. 
 *		 Takes input file and outpust as generates 10,000 bytes BUFFER Sized output sub files from inout file  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>


#define BUFFERSIZE 10000
#define CREAT_PERMISSION 00600
#define MAX_FILE_NAME_LENGTH 4096

void read_input_file(char *input_file, int fid_in,  char *program_file_name);
void create_output_file(char output_file[], char *program_file_name, int file_output_count, int fid_in, char *buffer_data, ssize_t input_buf_read_size);
void throw_error(int err_no, char *program_file_name, char *input_file, char *operation);
void check_input_args(int argc);
char *get_file_name_with_extension(char *input_file, int file_output_count);


int main(int argc, char **argv){
    check_input_args(argc);
   
    int fid_in = 0;
    char *program_file_name = argv[0];
    program_file_name += 2;

    /* Here opens input file to read and checks for error */ 
    if((fid_in = open(argv[1],O_RDONLY)) == -1 ) {
        throw_error(errno, program_file_name, argv[1], "open");
    } else {
        /* Here calls function to read input files by using buffer */ 
        read_input_file(argv[1], fid_in, program_file_name);

        close(fid_in);  
    }

    return 0;
}

/* Here reads opened input file to read buffer continuesly with buffer sized 10000 */
void read_input_file(char *input_file, int fid_in, char *program_file_name) {

    ssize_t input_buf_read_size;
    char *buffer_data = NULL;
    buffer_data = malloc((BUFFERSIZE)*sizeof(char));
    struct stat file_stats; 
    off_t file_offset;

    stat(input_file,&file_stats);

    int file_output_count = 0;
    /* Here loops through reads file and sends buffer data to create/open output file */
    while((input_buf_read_size = pread(fid_in, buffer_data, BUFFERSIZE, file_offset)) > 0) {

        if(input_buf_read_size < BUFFERSIZE && input_buf_read_size + file_offset != file_stats.st_size) {
            continue;
        } else {
            /* Here calls the function to create output files of serial extension */ 
            create_output_file(input_file, program_file_name, file_output_count, fid_in, buffer_data, input_buf_read_size);
            file_output_count++;
            file_offset += input_buf_read_size;
        }
    }

    free(buffer_data);
}

/* Here creates output file with serial extensions from provided inputs */
void create_output_file(char *input_file, char *program_file_name, int file_output_count, int fid_in, char *buffer_data, ssize_t input_buf_read_size) {

    int fid_out;
    ssize_t output_write_size;
    
    char *output_file = get_file_name_with_extension(input_file,file_output_count);
    /* Here opens/creates output files to write or checks for errors */
    if((fid_out = open(output_file, O_WRONLY | O_CREAT, CREAT_PERMISSION)) == -1 ) {
        close(fid_in);
        throw_error(errno, program_file_name, output_file, "open");
    } else {
        if(((output_write_size = pwrite(fid_out, buffer_data, input_buf_read_size, SEEK_SET)) == -1) || output_write_size != input_buf_read_size) {
            close(fid_in);
            close(fid_out);
            throw_error(errno, program_file_name, output_file, "write into");
        } else {
            close(fid_out);
            printf("%s\n",output_file);
        }
    }
} 

/* Here prepares output filename with provided inputs */
char *get_file_name_with_extension(char *input_file, int file_output_count) {
    char output_file[MAX_FILE_NAME_LENGTH] = "";
    strcat(strcat(output_file, input_file), ".");

    char buffer[256];
    snprintf(buffer, 10, "%d", file_output_count);
    return strcat(output_file, buffer);
}

/* Here shows most possible errors occured while running */
void throw_error(int err_no, char *program_file_name, char *input_file, char *operation) {
    printf("%s: Couldn't %s file %s: %s.\n",program_file_name, operation, input_file, strerror(err_no));
    exit(0);
}

/* Here checks for all possible user input arguments acceptance */
void check_input_args(int argc) {
    if(argc < 2) {
        printf("Too few arguments !! \nUsage : split inputfile \n");
        exit(0);
    } else if(argc > 2) {
        printf("Too many arguments !! \nUsage : split inputfile \n");
        exit(0);
    }
}
