/*======================================================================
 * mapx.h - definitions for map projection routines
 *
 *	2-July-1991 K.Knowles knowles@kryos.colorado.edu 303-492-0644
 *      15-Dec-1992 R.Swick added constants for ellipsoid projections
 *======================================================================*/
#ifndef mapx_h_
#define mapx_h_

/* 
 * useful macros
 */
#define NORMALIZE(lon) \
{ while (lon < -180) lon += 360; \
  while (lon >  180) lon -= 360; \
}

#define SQRT2 1.414213562373095
#define PI 3.141592653589793
/* radius of the earth in km, authalic sphere based on International datum */
#define Re  6371.228	/* radius of the earth in km */

#define RADIANS(t) ((t) * PI/180)
#define DEGREES(t) ((t) * 180/PI)

/*
 * map parameters structure
 */
typedef struct {
  float lat0, lon0, lat1, lon1;
  float Rg, sin_phi1, cos_phi1, sin_lam1, cos_lam1;
  float rotation, scale;
  float south, north, west, east;
  float center_lat, center_lon, label_lat, label_lon;
  float lat_interval, lon_interval;
  float T00, T01, T10, T11, u0, v0;
  int map_stradles_180;
  int cil_detail, bdy_detail, riv_detail;
  double equitorial_radius, flattening;
  double eccentricity, e2, e4, e6, qp, Rq, q1;
  double beta1, sin_beta1, cos_beta1, m1, D, phis, kz;  
  int (*geo_to_map)(float, float, float *, float *);
  int (*map_to_geo)(float, float, float *, float *);
  FILE *mpp_file;
  char *mpp_file_name;
} mapx_class;

/*
 * function prototypes
 */
mapx_class *init_mapx(char *map_file_name);
void close_mapx(mapx_class *this);
int within_mapx(mapx_class *this, float lat, float lon);
int forward_mapx(mapx_class *this, float lat, float lon, float *u, float *v);
int inverse_mapx(mapx_class *this, float u, float v, float *lat, float *lon);

#endif
