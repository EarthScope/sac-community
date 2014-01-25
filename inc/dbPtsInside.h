struct point{
   float x;
   float y;
};

struct line{
   struct point p1;
   struct point p2;
};



extern int inside(struct point t, struct point *p, int N);
