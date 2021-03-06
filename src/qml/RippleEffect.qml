import QtQuick 2.5

ShaderEffect {
    id: shaderEffect

    // Properties that will get bound to a uniform with the same name in the shader
    property color backgroundColor: "#10000000"
    property color spreadColor: "#80000000"
    property point normTouchPos
    property real widthToHeightRatio: height / width
    // Our animated uniform property
    property real spread: 0
    opacity: 0

    visible: GRAPHICAL_EFFECTS_LEVEL >= 2

    ParallelAnimation {
        id: touchStartAnimation
        UniformAnimator {
            uniform: "spread"; target: shaderEffect
            from: 0; to: 1
            duration: 1000; easing.type: Easing.InQuad
        }
        OpacityAnimator {
            target: shaderEffect
            from: 0; to: 1
            duration: 50; easing.type: Easing.InQuad
        }
    }

    ParallelAnimation {
        id: touchEndAnimation
        UniformAnimator {
            uniform: "spread"; target: shaderEffect
            from: spread; to: 1
            duration: 1000; easing.type: Easing.OutQuad
        }
        OpacityAnimator {
            target: shaderEffect
            from: 1; to: 0
            duration: 1000; easing.type: Easing.OutQuad
        }
    }

    // minimalistic shader for Min graphical effects level:
    fragmentShader: "
        varying mediump vec2 qt_TexCoord0;
        uniform lowp float qt_Opacity;
        uniform lowp vec4 backgroundColor;
        uniform lowp vec4 spreadColor;
        uniform mediump vec2 normTouchPos;
        uniform mediump float widthToHeightRatio;
        uniform mediump float spread;

        void main() {
            gl_FragColor = (backgroundColor + spreadColor * spread) * qt_Opacity;
        }
    "

    // load advanced shader for Mid or Max graphical effects level:
    Component.onCompleted: {
        if (GRAPHICAL_EFFECTS_LEVEL >= 2) {
            fragmentShader = "
                varying mediump vec2 qt_TexCoord0;
                uniform lowp float qt_Opacity;
                uniform lowp vec4 backgroundColor;
                uniform lowp vec4 spreadColor;
                uniform mediump vec2 normTouchPos;
                uniform mediump float widthToHeightRatio;
                uniform mediump float spread;

                void main() {
                    // Pin the touched position of the circle by moving the center as
                    // the radius grows. Both left and right ends of the circle should
                    // touch the item edges simultaneously.
                    mediump float radius = (0.5 + abs(0.5 - normTouchPos.x)) * 1.0 * spread;
                    mediump vec2 circleCenter =
                        normTouchPos + (vec2(0.5) - normTouchPos) * radius * 2.0;

                    // Calculate everything according to the x-axis assuming that
                    // the overlay is horizontal or square. Keep the aspect for the
                    // y-axis since we're dealing with 0..1 coordinates.
                    mediump float circleX = (qt_TexCoord0.x - circleCenter.x);
                    mediump float circleY = (qt_TexCoord0.y - circleCenter.y) * widthToHeightRatio;

                    // Use step to apply the color only if x2*y2 < r2.
                    lowp vec4 tapOverlay =
                        spreadColor * step(circleX*circleX + circleY*circleY, radius*radius);
                    gl_FragColor = (backgroundColor + tapOverlay) * qt_Opacity;
                }
            "
        }
    }

    function touchStart(x, y) {
        normTouchPos = Qt.point(x / width, y / height)
        touchEndAnimation.stop()
        touchStartAnimation.start()
    }
    function touchEnd() {
        touchStartAnimation.stop()
        touchEndAnimation.start()
    }
    function touchCancel() {
        touchStartAnimation.stop()
        touchEndAnimation.stop()
        spread = 0
        shaderEffect.opacity = 0
    }
}
