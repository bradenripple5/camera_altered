package com.media.camera.preview.slider;

import android.view.MotionEvent;
import android.widget.SeekBar;

public class SliderFilter implements SeekBar.OnSeekBarChangeListener {

    public interface Listener {
        void onThresholdChanged(float value);
    }

    private Listener mListener;

    public SliderFilter(Listener listener) {
        this.mListener = listener;
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        if (mListener != null && fromUser) {
            float normalized = progress / 100.0f; // assume slider max = 100
            mListener.onThresholdChanged(normalized);
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        // optional
    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        // optional
    }
}
