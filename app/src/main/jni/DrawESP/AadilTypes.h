#include <jni.h>

struct Vector2 {
	float x,y;
    Vector2(float X, float Y):x(X), y(Y){
    }
};

struct Vector3 {
    float x,y,z;
    Vector3(float X, float Y, float Z):x(X), y(Y), z(Z){
    }
};

struct Color {
	float r,g,b,a;
    Color(float R, float G, float B, float A):r(R), g(G), b(B), a(A){

    }
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;

    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(float _x, float _y, float _z, float _w)
            : x(_x), y(_y), z(_z), w(_w) {}
};


class AadilDrawing {
	private:
		JNIEnv *jnienv;
		jobject espview;
		jobject canvas;
		
	public:
		AadilDrawing() {
			jnienv = nullptr;
			espview = nullptr;
			canvas = nullptr;
		}
		
		AadilDrawing(JNIEnv *env, jobject esp, jobject cvs) {
			this->jnienv = env;
			this->espview = esp;
			this->canvas = cvs;
		}
		
		bool isValid() const {
			return (jnienv && espview && canvas);
		}
		
		int getWidth() const {
			if (isValid()) {
				jclass cvs = jnienv->GetObjectClass(canvas);
				jmethodID width = jnienv->GetMethodID(cvs, "getWidth", "()I");
				return jnienv->CallIntMethod(canvas, width);
			}
		}
		
		int getHeight() const {
			if (isValid()) {
				jclass cvs = jnienv->GetObjectClass(canvas);
				jmethodID height = jnienv->GetMethodID(cvs, "getHeight", "()I");
				return jnienv->CallIntMethod(canvas, height);
			}
		}
		
		void DrawLine(Color color, float thickness, Vector2 from, Vector2 to) {
			if (isValid()) {
				jclass cvs = jnienv->GetObjectClass(espview);
				jmethodID drawline = jnienv->GetMethodID(cvs, "DrawLine", "(Landroid/graphics/Canvas;IIIIFFFFF)V");
				jnienv->CallVoidMethod(espview, drawline, canvas, (int)color.a, (int)color.r, (int)color.g, (int)color.b,
				thickness, from.x, from.y, to.x, to.y);
			}
		}

    void DrawText(const char* text, Color color, float size, Vector2 pos) {
        if (!isValid()) return;

        jclass cls = jnienv->GetObjectClass(espview);
        jmethodID drawText = jnienv->GetMethodID(
                cls,
                "DrawText",
                "(Landroid/graphics/Canvas;Ljava/lang/String;IIIIFFF)V"
        );

        jstring jText = jnienv->NewStringUTF(text);

        jnienv->CallVoidMethod(
                espview,
                drawText,
                canvas,
                jText,
                (int)color.a,
                (int)color.r,
                (int)color.g,
                (int)color.b,
                size,
                pos.x,
                pos.y
        );

        jnienv->DeleteLocalRef(jText);
    }

    void DrawCircle(Color color, float thickness, Vector2 center, float radius) {
        if (!isValid()) return;

        jclass cls = jnienv->GetObjectClass(espview);
        jmethodID drawCircle = jnienv->GetMethodID(
                cls,
                "DrawCircle",
                "(Landroid/graphics/Canvas;IIIIFFFF)V"
        );

        jnienv->CallVoidMethod(
                espview,
                drawCircle,
                canvas,
                (int)color.a,
                (int)color.r,
                (int)color.g,
                (int)color.b,
                thickness,
                center.x,
                center.y,
                radius
        );
    }

    void DrawBox(Color color, float thickness,
                 Vector2 topMiddle, float width, float height,
                 int health, float distance) {

        if (!isValid()) return;

        jclass cls = jnienv->GetObjectClass(espview);
        if (!cls) return;

        jmethodID drawBox = jnienv->GetMethodID(
                cls,
                "DrawBoxFromTopMiddle",
                "(Landroid/graphics/Canvas;IIIIFFFFFIF)V"
        );
        if (!drawBox) return;

        jnienv->CallVoidMethod(
                espview,
                drawBox,
                canvas,
                (int)color.a,
                (int)color.r,
                (int)color.g,
                (int)color.b,
                thickness,
                topMiddle.x,
                topMiddle.y,
                width,
                height,
                health,
                distance
        );
    }




};
