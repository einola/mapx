/*======================================================================
 * matrix_io.c - programs to read and write matrix data objects
 *
 * 03/7/1997 brodzik brodzik@zamboni.colorado.edu 303-492-8263
 *
 * National Snow & Ice Data Center, University of Colorado, Boulder
 *
 *$Log: not supported by cvs2svn $
 * Revision 1.5  1997/03/22  22:53:48  brodzik
 * Walkthrough changes: added error condition checks,
 * changed some variable names.
 *
 * Revision 1.4  1997/03/07  19:42:42  brodzik
 * Took _data off names. Reran unit test.
 *
 * Revision 1.3  1997/03/07  19:32:44  brodzik
 * Added NSIDC standard file header.
 *
 * Revision 1.2  1997/02/27  21:57:38  brodzik
 * Unit tested.
 *
 * Revision 1.1  1997/02/27  21:54:29  brodzik
 * Initial revision
 *
 *======================================================================*/
#include <define.h>
#include <matrix.h>
#include <matrix_io.h>

static const char matrix_io_c_RCSID[]="$Header: /tmp_mnt/FILES/mapx/matrix_io.c,v 1.6 1997-09-26 17:36:41 brodzik Exp $";

#define ZERO_BYTES 0;

/*----------------------------------------------------------------------
 * read_matrix
 *
 *	input : file_name - complete data file name
 *		data - pointer to matrix
 *		rows,cols - dimensions of data grid
 *              element_size - size (in bytes) of one data element
 *
 *      output : data - returned with 2D matrix read from file_name
 *
 *      result : data is populated with matrix from file_name
 *               return value is number of bytes read, or
 *               0 in case of error, with error message written to stderr
 *
 *----------------------------------------------------------------------*/
size_t read_matrix (void **data, char *file_name, 
		    int rows, int cols, size_t element_size)
{
  int i;
  size_t nbytes, bytes_per_row;
  size_t bytes_read;
  FILE *fp;

  if (0 == rows || 0 == cols || 0 == element_size) {
    fprintf(stderr,"read_matrix: zero matrix descriptors: "\
	    "rows=%d, cols=%d, element_size=%ld\n",
	    rows,cols,element_size);
    return ZERO_BYTES;
  }

  if (NULL == data) {
    fprintf(stderr,"read_matrix: NULL data pointer\n");
    return ZERO_BYTES;
  }

  bytes_read=ZERO_BYTES;

  fp = fopen (file_name, "rb");
  if (fp == NULL) { 
    perror(file_name); 
    return ZERO_BYTES; 
  }

  bytes_per_row = cols*element_size;
  for (i=0; i < rows; i++)
    { nbytes = fread (data[i], 1, bytes_per_row, fp);
      if (nbytes != bytes_per_row) { 
	perror(file_name); 
	return ZERO_BYTES; 
      }
      bytes_read += nbytes;
    }

  fclose (fp);
  return bytes_read;
}



/*----------------------------------------------------------------------
 * write_matrix
 *
 *	input : file_name - complete data file name
 *		data - pointer to matrix
 *		rows,cols - dimensions of data grid
 *              element_size - size (in bytes) of one data element
 *
 *      output : n/a
 *
 *      result : data is written to file_name
 *               return value is number bytes written, or
 *               0 in case of error, with error message written to stderr
 *
 *----------------------------------------------------------------------*/
size_t write_matrix (char *file_name, void **data, 
		     int rows, int cols, size_t element_size)
{
  int i;
  size_t nbytes, bytes_per_row;
  size_t bytes_written;
  FILE *fp;

  if (0 == rows || 0 == cols || 0 == element_size) {
    fprintf(stderr,"write_matrix: zero matrix descriptors: "\
	    "rows=%d, cols=%d, element_size=%ld\n",
	    rows,cols,element_size);
    return ZERO_BYTES;
  }

  if (NULL == data) {
    fprintf(stderr,"write_matrix: NULL data pointer\n");
    return ZERO_BYTES;
  }

  bytes_written=ZERO_BYTES;

  fp = fopen (file_name, "wb");
  if (fp == NULL) { 
    perror(file_name); 
    return ZERO_BYTES; 
  }

  bytes_per_row = cols*element_size;
  for (i=0; i < rows; i++)
    { nbytes = fwrite (data[i], 1, bytes_per_row, fp);
      if (nbytes != bytes_per_row) { 
	perror(file_name); 
	return ZERO_BYTES; 
      }
      bytes_written += nbytes;
    }

  fclose (fp);
  return bytes_written;
}

/*----------------------------------------------------------------------
 *  initialize_matrix - allocates memory for and reads matrix object 
 *                      from external file
 * 
 *     input : *grid - pointer to (initialized) grid_class
 *             size - size in bytes of one matrix element
 *             *file_name - filename to read from (if NULL, matrix
 *                         initialized to zeroes)
 *             *object_name - string to use for verbose output and errors
 *             verbose - verbose flag
 * 
 *    output: memory is initialized to contents of filename when 
 *              filename is non-NULL, zeroes otherwise
 *    result: pointer to newly allocated and initialized matrix
 *----------------------------------------------------------------------*/
void **initialize_matrix (grid_class *grid,
			  size_t size,
			  const char *file_name, 
			  const char *object_name, 
			  bool verbose) {

  void **data;
  
  if (NULL == grid) {
    fprintf(stderr,"initialize_matrix: grid error, uninitialized grid\n");
    return NULL;
  }

  data = matrix(grid->rows,grid->cols,size,TRUE);
  if (NULL == data) {
    fprintf(stderr, "initialize_matrix: memory allocation error for %s.",
	    object_name);
    return NULL;
  }
  else if (verbose)
    fprintf(stderr,"> initialize_matrix: Allocated grid for %s.\n",
	    object_name);

  if (NULL != file_name) {
    if (0 == read_matrix (data, file_name, 
			  grid->rows,grid->cols,size)) {
      fprintf(stderr,"initialize_matrix: error reading %s data from %s.\n",
	      object_name, file_name);
      free(data);
      return NULL;
    }
    else if (verbose)
      fprintf(stderr,"> initialize_matrix: read %s data from %s.\n",
	      object_name, file_name);
  }

  return data;
}




