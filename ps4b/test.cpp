/***************************************************
 * Copyright Salma Boukdad & Jeannine Al Attal
 * @Authors: Salma Boukdad, Jeannine Al Attal
 * @Course: COMP.2040.203
 * @Date: 3/31/26
 * @Description: Unit tests for KeyFrame tweening.
 *   Tests cover interpolation correctness, direction,
 *   fixed frames, exact keyframe values, boundary
 *   clamping behavior, and AniPlayer-level rendering.
 **************************************************/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AniPlayerTests
#include <boost/test/unit_test.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include "KeyFrame.hpp"
#include "AniPlayer.hpp"

using sf::seconds;
using sf::Transform;
using sf::Vector2f;

// tolerance for floating point comparisons
static const float EPS = 0.001f;

// helper func - apply a keyframe's transform and get resulting
// position of origin used to verify translation values
static Vector2f framePos(AP::KeyFrame kf) {
    Transform t;
    kf.transform(t);
    return t.transformPoint({0.f, 0.f});
}

// helper func - apply transform and get position of a non-origin point
// used to detect scaling differences — a scaled frame maps (1,1) differently
static Vector2f framePosScaled(AP::KeyFrame kf) {
    Transform t;
    kf.transform(t);
    return t.transformPoint({1.f, 1.f});
}

// helper func - render an AniPlayer at a given time to an sf::Image
// advances the player to time t, draws to an offscreen texture, returns pixels
static sf::Image renderAt(AP::AniPlayer& player, sf::Time t,
                      unsigned int w, unsigned int h) {
    player.tween(t);
    sf::RenderTexture rt({w, h});
    rt.clear(sf::Color::Black);
    rt.draw(player);
    rt.display();
    return rt.getTexture().copyToImage();
}

// helper func - check if two sf::Images are pixel-identical
// compares raw pixel bytes across the entire image
static bool ImagesEqual(const sf::Image& a, const sf::Image& b) {
    if (a.getSize() != b.getSize()) return false;
    const auto* pa = a.getPixelsPtr();
    const auto* pb = b.getPixelsPtr();
    std::size_t total = static_cast<std::size_t>(a.getSize().x) *
                        static_cast<std::size_t>(a.getSize().y) * 4;
    for (std::size_t i = 0; i < total; i++) {
        if (pa[i] != pb[i]) return false;
    }
    return true;
}

/**********************************************************
* Tweening completeness
*    All fields (x, y, scaleX, scaleY, theta) must be
*    interpolated — not just position.
*    We check scale by transforming a non-origin point.
*    At t=5 (halfway), scale should be 2 not 1 or 3.
***********************************************************/
BOOST_AUTO_TEST_CASE(tween_completeness) {
    AP::KeyFrame a(nlohmann::json{{"t", 0}, {"x", 0}, {"y", 0},
        {"scale-x", 1}, {"scale-y", 1}, {"theta", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 10}, {"x", 0}, {"y", 0},
        {"scale-x", 3}, {"scale-y", 3}, {"theta", 0}});
    AP::KeyFrame mid = a.tween(b, seconds(5.f));
    // if scale is interpolated, point (1,1) maps to (2,2) at halfway
    Vector2f pos = framePosScaled(mid);
    BOOST_CHECK_CLOSE(pos.x, 2.f, EPS);
    BOOST_CHECK_CLOSE(pos.y, 2.f, EPS);
}

/***********************************************************
* Tweening direction
*    Interpolation must go from a toward b, not backward.
*    At t=2 (20% of the way from 0 to 10), x should be 20
*************************************************************/
BOOST_AUTO_TEST_CASE(tween_direction) {
    AP::KeyFrame a(nlohmann::json{{"t", 0}, {"x", 0}, {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 10}, {"x", 100}, {"y", 0}});
    AP::KeyFrame result = a.tween(b, seconds(2.f));
    Vector2f pos = framePos(result);
    // should be 20, not 80 or 0 or 100
    BOOST_CHECK_CLOSE(pos.x, 20.f, EPS);
}

/**********************************************************
* Fixed keyframe
*    A broken impl that always returns *this would give
*    x=0 at t=10 instead of x=100 (the alt frame's value).
*    We verify the end frame equals b exactly.
***********************************************************/
BOOST_AUTO_TEST_CASE(fixed_keyframe) {
    AP::KeyFrame a(nlohmann::json{{"t", 0}, {"x", 0}, {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 10}, {"x", 100}, {"y", 0}});
    // at t=10 (end), must equal b exactly — not stay at a
    Vector2f end = framePos(a.tween(b, seconds(10.f)));
    BOOST_CHECK_CLOSE(end.x, 100.f, EPS);
}

/*************************************************************
* Only keyframes
*    A broken step-function impl would return 0 at t<5
*    and 100 at t>=5 instead of interpolating smoothly.
*    We check t=1,3,7,9 to catch this.
************************************************************/
BOOST_AUTO_TEST_CASE(exact_keyframe_values) {
    AP::KeyFrame a(nlohmann::json{{"t", 0}, {"x", 0}, {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 10}, {"x", 100}, {"y", 100}});
    // each point should be exactly proportional to elapsed time
    BOOST_CHECK_CLOSE(framePos(a.tween(b, seconds(1.f))).x, 10.f, EPS);
    BOOST_CHECK_CLOSE(framePos(a.tween(b, seconds(3.f))).x, 30.f, EPS);
    BOOST_CHECK_CLOSE(framePos(a.tween(b, seconds(7.f))).x, 70.f, EPS);
    BOOST_CHECK_CLOSE(framePos(a.tween(b, seconds(9.f))).x, 90.f, EPS);
}

/***********************************************************
* Temporal boundary
*    Verifies correct values at the start, middle, and end
*    of the keyframe range — catching off-by-one errors.
***********************************************************/
BOOST_AUTO_TEST_CASE(temporal_boundary) {
    AP::KeyFrame a(nlohmann::json{{"t", 0}, {"x", 0}, {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 10}, {"x", 100}, {"y", 0}});
    // check t=0, t=5, t=10 for correct values
    BOOST_CHECK_CLOSE(framePos(a.tween(b, seconds(0.f))).x,   0.f, EPS);
    BOOST_CHECK_CLOSE(framePos(a.tween(b, seconds(5.f))).x,  50.f, EPS);
    BOOST_CHECK_CLOSE(framePos(a.tween(b, seconds(10.f))).x, 100.f, EPS);
}

/***********************************************************
* Multiple keyframes
*    Tweening through three keyframes — t=0 to t=5 to t=10.
*    At t=2.5 should be halfway between first and second.
*    At t=7.5 should be halfway between second and third.
***********************************************************/
BOOST_AUTO_TEST_CASE(multiple_keyframes) {
    AP::KeyFrame a(nlohmann::json{{"t", 0},  {"x", 0},   {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 5},  {"x", 50},  {"y", 50}});
    AP::KeyFrame c(nlohmann::json{{"t", 10}, {"x", 100}, {"y", 0}});

    // halfway between a and b
    Vector2f ab = framePos(a.tween(b, seconds(2.5f)));
    BOOST_CHECK_CLOSE(ab.x, 25.f, EPS);
    BOOST_CHECK_CLOSE(ab.y, 25.f, EPS);

    // halfway between b and c
    Vector2f bc = framePos(b.tween(c, seconds(7.5f)));
    BOOST_CHECK_CLOSE(bc.x, 75.f, EPS);
    BOOST_CHECK_CLOSE(bc.y, 25.f, EPS);
}

/***********************************************************
* Inbetween interpolation — position
*    Verifies linear interpolation at many points along
*    the timeline, not just endpoints or midpoint.
*    Checks all 9 intermediate integer seconds.
***********************************************************/
BOOST_AUTO_TEST_CASE(inbetween_position) {
    AP::KeyFrame a(nlohmann::json{{"t", 0},  {"x", 0},   {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 10}, {"x", 100}, {"y", 200}});

    // check 9 intermediate points — each should be exactly proportional
    for (int i = 1; i < 10; i++) {
        float t = static_cast<float>(i);
        Vector2f pos = framePos(a.tween(b, seconds(t)));
        BOOST_CHECK_CLOSE(pos.x, t * 10.f, EPS);
        BOOST_CHECK_CLOSE(pos.y, t * 20.f, EPS);
    }
}

/***********************************************************
* Inbetween interpolation — scale
*    Verifies scale is linearly interpolated between
*    keyframes, not just snapped to start or end value.
*    Uses framePosScaled to detect scale changes.
***********************************************************/
BOOST_AUTO_TEST_CASE(inbetween_scale) {
    AP::KeyFrame a(nlohmann::json{{"t", 0},  {"scale-x", 1}, {"scale-y", 1}});
    AP::KeyFrame b(nlohmann::json{{"t", 4},  {"scale-x", 5}, {"scale-y", 5}});

    // at t=1 (25% of way), scale should be 2
    Vector2f p1 = framePosScaled(a.tween(b, seconds(1.f)));
    BOOST_CHECK_CLOSE(p1.x, 2.f, EPS);

    // at t=2 (50% of way), scale should be 3
    Vector2f p2 = framePosScaled(a.tween(b, seconds(2.f)));
    BOOST_CHECK_CLOSE(p2.x, 3.f, EPS);

    // at t=3 (75% of way), scale should be 4
    Vector2f p3 = framePosScaled(a.tween(b, seconds(3.f)));
    BOOST_CHECK_CLOSE(p3.x, 4.f, EPS);
}

/***********************************************************
* Inbetween interpolation — AniPlayer level
*    Verifies that intermediate frames actually differ
*    from each other, catching impls that snap to
*    nearest keyframe instead of interpolating.
***********************************************************/
BOOST_AUTO_TEST_CASE(inbetween_aniplayer) {
    AP::AniPlayer player("videos/three-circle-b.json");
    sf::Vector2u size = player.windowSize();

    // render at three consecutive seconds
    sf::Image img1 = renderAt(player, seconds(1.f), size.x, size.y);
    sf::Image img2 = renderAt(player, seconds(2.f), size.x, size.y);
    sf::Image img3 = renderAt(player, seconds(3.f), size.x, size.y);

    // each frame should look different from the others
    BOOST_CHECK(!ImagesEqual(img1, img2));
    BOOST_CHECK(!ImagesEqual(img2, img3));
    BOOST_CHECK(!ImagesEqual(img1, img3));
}

/***********************************************************
* Temporal boundary — exact start and end times
*    At exactly t=start, must return start keyframe.
*    At exactly t=end, must return end keyframe.
***********************************************************/
BOOST_AUTO_TEST_CASE(boundary_exact_times) {
    AP::KeyFrame a(nlohmann::json{{"t", 3}, {"x", 10}, {"y", 10}});
    AP::KeyFrame b(nlohmann::json{{"t", 7}, {"x", 90}, {"y", 90}});

    // at exactly t=3, must equal a
    Vector2f start = framePos(a.tween(b, seconds(3.f)));
    BOOST_CHECK_CLOSE(start.x, 10.f, EPS);
    BOOST_CHECK_CLOSE(start.y, 10.f, EPS);

    // at exactly t=7, must equal b
    Vector2f end = framePos(a.tween(b, seconds(7.f)));
    BOOST_CHECK_CLOSE(end.x, 90.f, EPS);
    BOOST_CHECK_CLOSE(end.y, 90.f, EPS);
}

/***********************************************************
* Temporal boundary — non-zero start time
*    When first keyframe is not at t=0, interpolation
*    should still be correct relative to that start time.
***********************************************************/
BOOST_AUTO_TEST_CASE(boundary_nonzero_start) {
    AP::KeyFrame a(nlohmann::json{{"t", 5},  {"x", 0},   {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 15}, {"x", 100}, {"y", 100}});

    // at t=5 (start), should be 0
    Vector2f start = framePos(a.tween(b, seconds(5.f)));
    BOOST_CHECK_CLOSE(start.x, 0.f, EPS);

    // at t=10 (halfway between 5 and 15), should be 50
    Vector2f mid = framePos(a.tween(b, seconds(10.f)));
    BOOST_CHECK_CLOSE(mid.x, 50.f, EPS);
    BOOST_CHECK_CLOSE(mid.y, 50.f, EPS);

    // at t=15 (end), should be 100
    Vector2f end = framePos(a.tween(b, seconds(15.f)));
    BOOST_CHECK_CLOSE(end.x, 100.f, EPS);
}

/***********************************************************
* Temporal boundary — AniPlayer level
*    Verifies that at t=0 the first frame is shown,
*    that the final frame holds after animation ends,
*    and that t=0 and t=5 look different.
***********************************************************/
BOOST_AUTO_TEST_CASE(boundary_aniplayer) {
    AP::AniPlayer player("videos/three-circle-b.json");
    sf::Vector2u size = player.windowSize();

    // t=0 rendered twice must be identical (deterministic)
    sf::Image img0a = renderAt(player, seconds(0.f), size.x, size.y);
    sf::Image img0b = renderAt(player, seconds(0.f), size.x, size.y);
    BOOST_CHECK(ImagesEqual(img0a, img0b));

    // final frame (t=5) held at t=100 must equal t=5
    sf::Image img5   = renderAt(player, seconds(5.f),   size.x, size.y);
    sf::Image img100 = renderAt(player, seconds(100.f), size.x, size.y);
    BOOST_CHECK(ImagesEqual(img5, img100));

    // t=0 and t=5 must differ — animation should have progressed
    BOOST_CHECK(!ImagesEqual(img0a, img5));
}

/***********************************************************
* Temporal boundary — small time increments
*    Verifies smooth interpolation at very small steps,
*    catching impls that round or snap to keyframes.
***********************************************************/
BOOST_AUTO_TEST_CASE(boundary_small_increments) {
    AP::KeyFrame a(nlohmann::json{{"t", 0},  {"x", 0},   {"y", 0}});
    AP::KeyFrame b(nlohmann::json{{"t", 10}, {"x", 100}, {"y", 0}});

    // check three very small increments — each should be exactly 1% of range
    Vector2f p01 = framePos(a.tween(b, seconds(0.1f)));
    Vector2f p02 = framePos(a.tween(b, seconds(0.2f)));
    Vector2f p03 = framePos(a.tween(b, seconds(0.3f)));

    BOOST_CHECK_CLOSE(p01.x, 1.f, EPS);
    BOOST_CHECK_CLOSE(p02.x, 2.f, EPS);
    BOOST_CHECK_CLOSE(p03.x, 3.f, EPS);

    // each increment must be strictly greater than the previous
    BOOST_CHECK(p01.x < p02.x);
    BOOST_CHECK(p02.x < p03.x);
}
