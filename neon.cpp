#include <jni.h>
#include <android/bitmap.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

static uint8_t cb(float x){ return static_cast<uint8_t>(std::clamp(x,0.f,255.f)+.5f); }
static float cw(float x){ x=std::fabs(x); if(x<1) return 1-2.5f*x*x+1.5f*x*x*x; if(x<2) return 2-4*x+2.5f*x*x-.5f*x*x*x; return 0; }
static void sample(const uint8_t* s,int w,int h,float x,float y,int mode,float o[4]){
 int bx=(int)std::floor(x), by=(int)std::floor(y); float sum[4]={}, tw=0;
 if(mode==0){ int x0=std::clamp(bx,0,w-1),y0=std::clamp(by,0,h-1),x1=std::clamp(bx+1,0,w-1),y1=std::clamp(by+1,0,h-1); float fx=std::clamp(x-bx,0.f,1.f),fy=std::clamp(y-by,0.f,1.f); const uint8_t *a=s+(y0*w+x0)*4,*b=s+(y0*w+x1)*4,*c=s+(y1*w+x0)*4,*d=s+(y1*w+x1)*4; for(int k=0;k<4;k++){float t=a[k]*(1-fx)+b[k]*fx,u=c[k]*(1-fx)+d[k]*fx;o[k]=t*(1-fy)+u*fy;} return; }
 for(int j=-1;j<=2;j++) for(int i=-1;i<=2;i++){ int xx=std::clamp(bx+i,0,w-1),yy=std::clamp(by+j,0,h-1); float z=cw(x-(bx+i))*cw(y-(by+j)); const uint8_t*p=s+(yy*w+xx)*4; for(int k=0;k<4;k++)sum[k]+=p[k]*z; tw+=z; }
 for(int k=0;k<4;k++) o[k]=std::fabs(tw)>1e-6f?sum[k]/tw:0;
}
extern "C" JNIEXPORT jint JNICALL Java_com_neon_upscaler_MainActivity_scaleNative(JNIEnv* e,jobject,jobject srcBmp,jobject dstBmp,jint mode,jfloat sharp){
 AndroidBitmapInfo si{},di{}; if(AndroidBitmap_getInfo(e,srcBmp,&si)||AndroidBitmap_getInfo(e,dstBmp,&di)) return -1;
 if(si.format!=ANDROID_BITMAP_FORMAT_RGBA_8888||di.format!=ANDROID_BITMAP_FORMAT_RGBA_8888) return -2;
 void *sp=nullptr,*dp=nullptr; if(AndroidBitmap_lockPixels(e,srcBmp,&sp))return -3; if(AndroidBitmap_lockPixels(e,dstBmp,&dp)){AndroidBitmap_unlockPixels(e,srcBmp);return -4;}
 const auto* src=(const uint8_t*)sp; auto* dst=(uint8_t*)dp; int sw=si.width,sh=si.height,dw=di.width,dh=di.height; sharp=std::clamp(sharp,0.f,.5f); float xs=(float)sw/dw,ys=(float)sh/dh;
 for(int y=0;y<dh;y++)for(int x=0;x<dw;x++){float sx=(x+.5f)*xs-.5f,sy=(y+.5f)*ys-.5f,c[4]; sample(src,sw,sh,sx,sy,mode,c); if(sharp>0&&x>0&&y>0&&x+1<dw&&y+1<dh){float l[4],r[4],u[4],d[4];sample(src,sw,sh,sx-xs,sy,mode,l);sample(src,sw,sh,sx+xs,sy,mode,r);sample(src,sw,sh,sx,sy-ys,mode,u);sample(src,sw,sh,sx,sy+ys,mode,d);for(int k=0;k<3;k++)c[k]+=sharp*(c[k]-(l[k]+r[k]+u[k]+d[k])*.25f);} uint8_t*out=dst+(y*dw+x)*4;for(int k=0;k<4;k++)out[k]=cb(c[k]);}
 AndroidBitmap_unlockPixels(e,dstBmp);AndroidBitmap_unlockPixels(e,srcBmp);return 0;
}
