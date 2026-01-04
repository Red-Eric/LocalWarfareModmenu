package com.android.support;
import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Handler;
import android.graphics.Color;
import android.graphics.PorterDuff;

public class ESPView extends View {

	private Paint strokePaint;
	private int FrameDelay = 33;

	public ESPView(Context context) {
		super(context);
		strokePaint = new Paint();
		strokePaint.setStyle(Paint.Style.STROKE);
		strokePaint.setAntiAlias(true);
		setFocusableInTouchMode(false);
		final Handler framehandler = new Handler();
		framehandler.postDelayed(new Runnable() {
				@Override
				public void run() {
					invalidate();
					framehandler.postDelayed(this, FrameDelay);
				}
			}, FrameDelay);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
		Menu.Draw(this, canvas);
	}

	public void DrawLine(Canvas cvs, int a, int r, int g, int b, float strokewidth, float fromX, float fromY, float toX, float toY) {
		strokePaint.setColor(Color.argb(a,r,g,b));
		strokePaint.setStrokeWidth(strokewidth);
		cvs.drawLine(fromX, fromY, toX, toY, strokePaint);
	}

    public void DrawText(Canvas cvs, String text, int a, int r, int g, int b,
                         float textSize, float x, float y) {
        strokePaint.setColor(Color.argb(a, r, g, b));
        strokePaint.setTextSize(textSize);
        strokePaint.setStyle(Paint.Style.FILL);
        strokePaint.setTextAlign(Paint.Align.LEFT);
        cvs.drawText(text, x, y, strokePaint);
    }

    public void DrawCircle(Canvas cvs, int a, int r, int g, int b,
                           float strokeWidth, float cx, float cy, float radius) {
        strokePaint.setColor(Color.argb(a, r, g, b));
        strokePaint.setStrokeWidth(strokeWidth);
        strokePaint.setStyle(Paint.Style.STROKE);
        cvs.drawCircle(cx, cy, radius, strokePaint);
    }


    public void DrawBoxFromTopMiddle(
            Canvas cvs,
            int a, int r, int g, int b,
            float strokeWidth,
            float topMiddleX,
            float topMiddleY,
            float width,
            float height,
            int health,
            float distance
    ) {
        // ===== BOX =====
        strokePaint.setColor(Color.argb(a, r, g, b));
        strokePaint.setStrokeWidth(strokeWidth);
        strokePaint.setStyle(Paint.Style.STROKE);

        float halfW = width * 0.5f;

        float left   = topMiddleX - halfW;
        float right  = topMiddleX + halfW;
        float top    = topMiddleY;
        float bottom = topMiddleY + height;

        cvs.drawLine(left, top, right, top, strokePaint);
        cvs.drawLine(left, bottom, right, bottom, strokePaint);
        cvs.drawLine(left, top, left, bottom, strokePaint);
        cvs.drawLine(right, top, right, bottom, strokePaint);

        // ===== HEALTH BAR =====
        if (health < 0) health = 0;
        if (health > 100) health = 100;

        int hpColor;
        if (health >= 70)
            hpColor = Color.GREEN;
        else if (health >= 45)
            hpColor = Color.YELLOW;
        else
            hpColor = Color.RED;

        float barWidth = strokeWidth * 2.0f;
        float barLeft  = left - barWidth - 3.0f;
        float barRight = barLeft + barWidth;

        float hpHeight = (health / 100.0f) * height;
        float hpTop    = bottom - hpHeight;

        strokePaint.setStyle(Paint.Style.FILL);
        strokePaint.setColor(Color.BLACK);
        cvs.drawRect(barLeft, top, barRight, bottom, strokePaint);

        strokePaint.setColor(hpColor);
        cvs.drawRect(barLeft, hpTop, barRight, bottom, strokePaint);

        // ===== DISTANCE TEXT =====
        strokePaint.setColor(Color.WHITE);
        strokePaint.setTextSize(24.0f);

        String distText = String.format("%.1f m", distance);
        float textWidth = strokePaint.measureText(distText);

        float textX = topMiddleX - (textWidth * 0.5f);
        float textY = bottom + 25.0f;

        cvs.drawText(distText, textX, textY, strokePaint);
    }












}

