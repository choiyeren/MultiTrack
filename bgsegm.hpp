#ifndef __OPENCV_BGSEGM_HPP__
#define __OPENCV_BGSEGM_HPP__

#include "opencv2/video.hpp"

#ifdef __cplusplus

/** @defgroup bgsegm Improved Background-Foreground Segmentation Methods
*/

namespace cv
{
namespace bgsegm
{

//! @addtogroup bgsegm
//! @{

/** @brief Gaussian Mixture-based Background/Foreground Segmentation Algorithm.

The class implements the algorithm described in @cite KB2001 .
 */
class CV_EXPORTS_W BackgroundSubtractorMOG : public BackgroundSubtractor
{
public:
    CV_WRAP virtual int getHistory() const = 0;
    CV_WRAP virtual void setHistory(int nframes) = 0;

    CV_WRAP virtual int getNMixtures() const = 0;
    CV_WRAP virtual void setNMixtures(int nmix) = 0;

    CV_WRAP virtual double getBackgroundRatio() const = 0;
    CV_WRAP virtual void setBackgroundRatio(double backgroundRatio) = 0;

    CV_WRAP virtual double getNoiseSigma() const = 0;
    CV_WRAP virtual void setNoiseSigma(double noiseSigma) = 0;
};

/** @brief Creates mixture-of-gaussian background subtractor

@param history Length of the history.
@param nmixtures Number of Gaussian mixtures.
@param backgroundRatio Background ratio.
@param noiseSigma Noise strength (standard deviation of the brightness or each color channel). 0
means some automatic value.
 */
CV_EXPORTS_W Ptr<BackgroundSubtractorMOG>
    createBackgroundSubtractorMOG(int history=200, int nmixtures=5,
                                  double backgroundRatio=0.7, double noiseSigma=0);


/** @brief Background Subtractor module based on the algorithm given in @cite Gold2012 .

 Takes a series of images and returns a sequence of mask (8UC1)
 images of the same size, where 255 indicates Foreground and 0 represents Background.
 This class implements an algorithm described in "Visual Tracking of Human Visitors under
 Variable-Lighting Conditions for a Responsive Audio Art Installation," A. Godbehere,
 A. Matsukawa, K. Goldberg, American Control Conference, Montreal, June 2012.
 */
class CV_EXPORTS_W BackgroundSubtractorGMG : public BackgroundSubtractor
{
public:
    /** @brief Returns total number of distinct colors to maintain in histogram.
    */
    CV_WRAP virtual int getMaxFeatures() const = 0;
    /** @brief Sets total number of distinct colors to maintain in histogram.
    */
    CV_WRAP virtual void setMaxFeatures(int maxFeatures) = 0;

    /** @brief Returns the learning rate of the algorithm.

    It lies between 0.0 and 1.0. It determines how quickly features are "forgotten" from
    histograms.
     */
    CV_WRAP virtual double getDefaultLearningRate() const = 0;
    /** @brief Sets the learning rate of the algorithm.
    */
    CV_WRAP virtual void setDefaultLearningRate(double lr) = 0;

    /** @brief Returns the number of frames used to initialize background model.
    */
    CV_WRAP virtual int getNumFrames() const = 0;
    /** @brief Sets the number of frames used to initialize background model.
    */
    CV_WRAP virtual void setNumFrames(int nframes) = 0;

    /** @brief Returns the parameter used for quantization of color-space.

    It is the number of discrete levels in each channel to be used in histograms.
     */
    CV_WRAP virtual int getQuantizationLevels() const = 0;
    /** @brief Sets the parameter used for quantization of color-space
    */
    CV_WRAP virtual void setQuantizationLevels(int nlevels) = 0;

    /** @brief Returns the prior probability that each individual pixel is a background pixel.
    */
    CV_WRAP virtual double getBackgroundPrior() const = 0;
    /** @brief Sets the prior probability that each individual pixel is a background pixel.
    */
    CV_WRAP virtual void setBackgroundPrior(double bgprior) = 0;

    /** @brief Returns the kernel radius used for morphological operations
    */
    CV_WRAP virtual int getSmoothingRadius() const = 0;
    /** @brief Sets the kernel radius used for morphological operations
    */
    CV_WRAP virtual void setSmoothingRadius(int radius) = 0;

    /** @brief Returns the value of decision threshold.

    Decision value is the value above which pixel is determined to be FG.
     */
    CV_WRAP virtual double getDecisionThreshold() const = 0;
    /** @brief Sets the value of decision threshold.
    */
    CV_WRAP virtual void setDecisionThreshold(double thresh) = 0;

    /** @brief Returns the status of background model update
    */
    CV_WRAP virtual bool getUpdateBackgroundModel() const = 0;
    /** @brief Sets the status of background model update
    */
    CV_WRAP virtual void setUpdateBackgroundModel(bool update) = 0;

    /** @brief Returns the minimum value taken on by pixels in image sequence. Usually 0.
    */
    CV_WRAP virtual double getMinVal() const = 0;
    /** @brief Sets the minimum value taken on by pixels in image sequence.
    */
    CV_WRAP virtual void setMinVal(double val) = 0;

    /** @brief Returns the maximum value taken on by pixels in image sequence. e.g. 1.0 or 255.
    */
    CV_WRAP virtual double getMaxVal() const = 0;
    /** @brief Sets the maximum value taken on by pixels in image sequence.
    */
    CV_WRAP virtual void setMaxVal(double val) = 0;
};

/** @brief Creates a GMG Background Subtractor

@param initializationFrames number of frames used to initialize the background models.
@param decisionThreshold Threshold value, above which it is marked foreground, else background.
 */
CV_EXPORTS_W Ptr<BackgroundSubtractorGMG> createBackgroundSubtractorGMG(int initializationFrames=120,
                                                                        double decisionThreshold=0.8);

/** @brief Background subtraction based on counting.

  About as fast as MOG2 on a high end system.
  More than twice faster than MOG2 on cheap hardware (benchmarked on Raspberry Pi3).
*/
class CV_EXPORTS_W BackgroundSubtractorCNT  : public BackgroundSubtractor
{
public:
    // BackgroundSubtractor interface
    CV_WRAP virtual void apply(InputArray image, OutputArray fgmask, double learningRate=-1) = 0;
    CV_WRAP virtual void getBackgroundImage(OutputArray backgroundImage) const = 0;

    /** @brief Returns number of frames with same pixel color to consider stable.
    */
    CV_WRAP virtual int getMinPixelStability() const = 0;
    /** @brief Sets the number of frames with same pixel color to consider stable.
    */
    CV_WRAP virtual void setMinPixelStability(int value) = 0;

    /** @brief Returns maximum allowed credit for a pixel in history.
    */
    CV_WRAP virtual int getMaxPixelStability() const = 0;
    /** @brief Sets the maximum allowed credit for a pixel in history.
    */
    CV_WRAP virtual void setMaxPixelStability(int value) = 0;

    /** @brief Returns if we're giving a pixel credit for being stable for a long time.
    */
    CV_WRAP virtual bool getUseHistory() const = 0;
    /** @brief Sets if we're giving a pixel credit for being stable for a long time.
    */
    CV_WRAP virtual void setUseHistory(bool value) = 0;

    /** @brief Returns if we're parallelizing the algorithm.
    */
    CV_WRAP virtual bool getIsParallel() const = 0;
    /** @brief Sets if we're parallelizing the algorithm.
    */
    CV_WRAP virtual void setIsParallel(bool value) = 0;
};

}
}

#endif
#endif
