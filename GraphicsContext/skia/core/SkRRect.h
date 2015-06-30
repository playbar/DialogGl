/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkRRect_DEFINED
#define SkRRect_DEFINED

#include "SkRect.h"
#include "SkPoint.h"

class SkPath;
class SkMatrix;


class SK_API SkRRect {
public:
    /**
     * Enum to capture the various possible subtypes of RR. Accessed
     * by type(). The subtypes become progressively less restrictive.
     */
    enum Type {
        // !< Internal indicator that the sub type must be computed.
        kUnknown_Type = -1,

        // !< The RR is empty
        kEmpty_Type,

        //!< The RR is actually a (non-empty) rect (i.e., at least one radius
        //!< at each corner is zero)
        kRect_Type,

        //!< The RR is actually a (non-empty) oval (i.e., all x radii are equal
        //!< and >= width/2 and all the y radii are equal and >= height/2
        kOval_Type,

        //!< The RR is non-empty and all the x radii are equal & all y radii
        //!< are equal but it is not an oval (i.e., there are lines between
        //!< the curves) nor a rect (i.e., both radii are non-zero)
        kSimple_Type,

        //!< A fully general (non-empty) RR. Some of the x and/or y radii are
        //!< different from the others and there must be one corner where
        //!< both radii are non-zero.
        kComplex_Type,
    };

    /**
     * Returns the RR's sub type.
     */
    Type getType() const {

        if (kUnknown_Type == fType) {
            this->computeType();
        }
        return fType;
    }

    Type type() const { return this->getType(); }

    inline bool isEmpty() const { return kEmpty_Type == this->getType(); }
    inline bool isRect() const { return kRect_Type == this->getType(); }
    inline bool isOval() const { return kOval_Type == this->getType(); }
    inline bool isSimple() const { return kSimple_Type == this->getType(); }
    inline bool isComplex() const { return kComplex_Type == this->getType(); }

    float width() const { return fRect.width(); }
    float height() const { return fRect.height(); }

    /**
     * Set this RR to the empty rectangle (0,0,0,0) with 0 x & y radii.
     */
    void setEmpty() {
        fRect.setEmpty();
        memset(fRadii, 0, sizeof(fRadii));
        fType = kEmpty_Type;
    }

    /**
     * Set this RR to match the supplied rect. All radii will be 0.
     */
    void setRect(const SkRect& rect) {
        if (rect.isEmpty()) {
            this->setEmpty();
            return;
        }

        fRect = rect;
        memset(fRadii, 0, sizeof(fRadii));
        fType = kRect_Type;

    }

    /**
     * Set this RR to match the supplied oval. All x radii will equal half the
     * width and all y radii will equal half the height.
     */
    void setOval(const SkRect& oval) {
        if (oval.isEmpty()) {
            this->setEmpty();
            return;
        }

        float xRad = SkScalarHalf(oval.width());
        float yRad = SkScalarHalf(oval.height());

        fRect = oval;
        for (int i = 0; i < 4; ++i) {
            fRadii[i].set(xRad, yRad);
        }
        fType = kOval_Type;

    }

    /**
     * Initialize the RR with the same radii for all four corners.
     */
    void setRectXY(const SkRect& rect, float xRad, float yRad);

    /**
     * Initialize the RR with potentially different radii for all four corners.
     */
    void setRectRadii(const SkRect& rect, const SkVector radii[4]);

    // The radii are stored in UL, UR, LR, LL order.
    enum Corner {
        kUpperLeft_Corner,
        kUpperRight_Corner,
        kLowerRight_Corner,
        kLowerLeft_Corner
    };

    const SkRect& rect() const { return fRect; }
    const SkVector& radii(Corner corner) const { return fRadii[corner]; }
    const SkRect& getBounds() const { return fRect; }

    /**
     *  When a rrect is simple, all of its radii are equal. This returns one
     *  of those radii. This call requires the rrect to be non-complex.
     */
    const SkVector& getSimpleRadii() const {
        return fRadii[0];
    }

    friend bool operator==(const SkRRect& a, const SkRRect& b) {
        return a.fRect == b.fRect &&
               SkScalarsEqual(a.fRadii[0].asScalars(),
                              b.fRadii[0].asScalars(), 8);
    }

    friend bool operator!=(const SkRRect& a, const SkRRect& b) {
        return a.fRect != b.fRect ||
               !SkScalarsEqual(a.fRadii[0].asScalars(),
                               b.fRadii[0].asScalars(), 8);
    }

    /**
     *  Call inset on the bounds, and adjust the radii to reflect what happens
     *  in stroking: If the corner is sharp (no curvature), leave it alone,
     *  otherwise we grow/shrink the radii by the amount of the inset. If a
     *  given radius becomes negative, it is pinned to 0.
     *
     *  It is valid for dst == this.
     */
    void inset(float dx, float dy, SkRRect* dst) const;

    void inset(float dx, float dy) {
        this->inset(dx, dy, this);
    }

    /**
     *  Call outset on the bounds, and adjust the radii to reflect what happens
     *  in stroking: If the corner is sharp (no curvature), leave it alone,
     *  otherwise we grow/shrink the radii by the amount of the inset. If a
     *  given radius becomes negative, it is pinned to 0.
     *
     *  It is valid for dst == this.
     */
    void outset(float dx, float dy, SkRRect* dst) const {
        this->inset(-dx, -dy, dst);
    }
    void outset(float dx, float dy) {
        this->inset(-dx, -dy, this);
    }

    /**
     *  Returns true if 'rect' is wholy inside the RR, and both
     *  are not empty.
     */
    bool contains(const SkRect& rect) const;

    enum {
        kSizeInMemory = 12 * sizeof(float)
    };

    /**
     *  Write the rrect into the specified buffer. This is guaranteed to always
     *  write kSizeInMemory bytes, and that value is guaranteed to always be
     *  a multiple of 4. Return kSizeInMemory.
     */
    size_t writeToMemory(void* buffer) const;

    /**
     * Reads the rrect from the specified buffer
     *
     * If the specified buffer is large enough, this will read kSizeInMemory bytes,
     * and that value is guaranteed to always be a multiple of 4.
     *
     * @param buffer Memory to read from
     * @param length Amount of memory available in the buffer
     * @return number of bytes read (must be a multiple of 4) or
     *         0 if there was not enough memory available
     */
    size_t readFromMemory(const void* buffer, size_t length);

    /**
     *  Transform by the specified matrix, and put the result in dst.
     *
     *  @param matrix SkMatrix specifying the transform. Must only contain
     *      scale and/or translate, or this call will fail.
     *  @param dst SkRRect to store the result. It is an error to use this,
     *      which would make this function no longer const.
     *  @return true on success, false on failure. If false, dst is unmodified.
     */
    bool transform(const SkMatrix& matrix, SkRRect* dst) const;

private:
    SkRect fRect;
    // Radii order is UL, UR, LR, LL. Use Corner enum to index into fRadii[]
    SkVector fRadii[4];
    mutable Type fType;
    // TODO: add padding so we can use memcpy for flattening and not copy
    // uninitialized data

    void computeType() const;
    bool checkCornerContainment(float x, float y) const;

    // to access fRadii directly
    friend class SkPath;
};

#endif
