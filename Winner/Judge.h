/**
 * Copyright (C) 2018 Shanghai YOUWAN Science & Technology Co., Ltd.
 * All rights reserved.
 *
 * This document is the property of Shanghai YOUWAN Science & Technology Co., Ltd.
 * It is considered confidential and proprietary.
 *
 * This document may not be reproduced or transmitted in any form,
 * in whole or in part, without the express written permission of
 * Shanghai YOUWAN Science & Technology Co., Ltd.
 */
#ifndef PAGAMES_WINNER_JUDGE_H
#define PAGAMES_WINNER_JUDGE_H

#include "WinnerPokerCommon.h"
#include <map>
#include <unordered_map>
#include <vector>
PAGAMES_WINNER_POKER_BEGIN

struct HandsCategoryModel
{
    HandsCategory handsCategory; //牌型
    size_t        weight;        //权重
    size_t        size;          //数量
};

class Judge
{
private:
    /** 这一圈的牌型 */
    struct CurrentHandsCategory
    {
        HandsCategoryModel  handsCategory; //牌型
        std::vector<size_t> hands;         //这一圈第一个人打出的牌，决定了本圈牌型
    };

public:
    Judge()              = default;
    Judge(const Judge &) = delete;
    Judge &operator=(const Judge &) = delete;

#pragma mark - 单例
    static Judge &getInstance();

#pragma mark - 判定
    /**
     * 根据传入的手牌，判定是什么牌型
     * @param hands 待判定手牌
     * @return 牌型
     */
    HandsCategoryModel judgeHandsCategory(const std::vector<size_t> &hands) const;

    /**
     * 根据传入的手牌，判定用户是否已经走投无路，只能选择要不起
     * @param hands 待判定手牌
     * @return 是否只能要不起
     */
    bool isPass(const std::vector<size_t> &hands);

    /**
     * 根据传入的手牌，判定用户是否可以出牌
     * @param hands 想要出的牌
     * @param isStartingHand 是不是起手牌，如果是整个牌局出的第一手牌，需要判定♥️3必出
     * @return 是否可以出牌
     */
    bool canPlay(const std::vector<size_t> &hands, bool isStartingHand = false) const;

    /**
     * 是否在传入的手牌中包含♥️3
     * @param hands 用户当前的整个手牌
     * @return 是否包含♥️3
     */
    bool isContainsThreeOfHearts(const std::vector<size_t> &hands) const;

#pragma mark - 提示
    /**
     * 智能提示，根据给 CurrentHandsCategory 传入的数组判断是想要首出提示还是跟牌提示
     * 当 setCurrentHandsCategory 传入空数组时，认为当前玩家首出，传入非空认为玩家跟牌
     * @param hands 玩家整个手牌
     * @param isStartingHand 是不是起手牌，如果是整个牌局出的第一手牌，需要判定♥️3必出
     * @return 手牌中符合出牌意图的某个组合，也有可能是空
     */
    std::vector<size_t> intentions(const std::vector<size_t> &hands, bool isStartingHand = false);

    /**
     * 应该提示最大单牌
     * 在2018年 8月27日 星期一，测试提出想要修改下家报单的逻辑
     * 当下家报单，如果首出提示是提示的单牌，那么应该提示最大的单牌，而不是最小的单牌
     * @param hands 玩家整个手牌
     */
    void shouldHintTheHighestSingleCard(const std::vector<size_t> &hands);

#pragma mark - 排序 & 重置索引
    /**
     * 重新排列打出的牌。
     *
     * @param hands 打出的牌。
     * @return 排列好的按照牌型排列的牌。
     */
    std::vector<size_t> rearrangeHands(const std::vector<size_t> &hands) const;

    /**
     * 在2018年 8月27日 星期一，测试提出想在把提示的牌点下去之后重置提示索引，想从头开始提示
     * 故开放本方法重置提示索引，调用后点击提示会从头开始
     */
    void reindex();

#pragma mark - 转换手牌
    /**
     * 转换手牌，将服务端传回的手牌数据结构转换为跑得快客户端约定的手牌数据结构
     * @param hands 服务端手牌的数据结构
     * @return 客户端约定的手牌数据结构
     */

    /**
     * 恢复手牌，将跑得快客户端约定的手牌数据结构恢复为服务端使用的手牌数据结构
     * @param hands 客户端约定的手牌数据结构
     * @return 服务端手牌的数据结构
     */

#pragma mark - getter & setter
    /**
     * 设置决定当前这一圈牌型的牌。
     * 如果设置为空的话说明当前用户拥有决定权，可以出符合牌型的任意牌。
     *
     * @param weight 决定出牌权重的牌数组（上家出牌）。
     * @param handsCategory 决定出牌及展示牌型的牌数组（本小轮首家出牌）。
     */
    void setCurrentHandsCategory(const std::vector<size_t> &weight, const std::vector<size_t> &handsCategory);

    /**
     * 设置当前的手牌，当玩家手牌发生变化时，应将手牌传给Judge，用在判断出牌是是否含有💣
     * 用以当💣不可拆，作出正确的能否出牌判定
     * @param hands
     */
    void setCurrentHands(const std::vector<size_t> &hands);

private:
#pragma mark - 私有变量
    /** 持有这一圈的牌型，用以判断能否跟牌和提示跟牌 */
    CurrentHandsCategory _currentHandsCategory{};

    /** 持有这一圈玩家的手牌，用以当💣不可拆，作出正确的能否出牌判定 */
    std::vector<size_t> _currentHands;

    /** 提示中必须包含的牌，可能是♥️3，也有可能是用户传入的手牌中最小的牌型 */
    size_t _target = 0;

    /** 持有分析好的首出提示数组 */
    std::vector<std::vector<size_t>> _cardIntentions{};

    /** 持有首出提示数组的迭代器，记录上一次提示的位置 */
    std::vector<std::vector<size_t>>::iterator _iteratorIntentions;

    /** 是否需要重新计算首出提示 */
    bool _needRecalculateIntentions = true;

    /** 持有分析好的跟牌提示数组 */
    std::vector<std::vector<size_t>> _cardHint{};

    /** 持有跟牌提示数组的迭代器，记录上一次提示的位置 */
    std::vector<std::vector<size_t>>::iterator _iteratorHint;

    /** 是否需要重新计算跟牌提示 */
    bool _needRecalculateHint = true;

private:
#pragma mark - 私有函数
    std::multimap<size_t, size_t> getRanksMultimap(const std::vector<size_t> &hands,
                                                   bool                       isThreeOfHeartsFirst = false) const;

    std::vector<size_t> getCardRanks(const std::vector<size_t> &hands) const;

public:
    template <typename T> std::unordered_map<size_t, size_t> zip(const T &t) const;

private:
    std::vector<size_t> unzip(const std::unordered_map<size_t, size_t> &zipped) const;

    std::vector<size_t> unzip(const std::unordered_map<size_t, size_t> &zipped, size_t ignore) const;

    std::vector<size_t> filter3(const std::unordered_map<size_t, size_t> &others, bool canSplit3) const;

    std::unordered_map<size_t, size_t> filterA(const std::unordered_map<size_t, size_t> &ranks) const;

    std::unordered_map<size_t, size_t> filterConventionalBomb(const std::unordered_map<size_t, size_t> &ranks) const;

    std::unordered_map<size_t, size_t> filterBombs(const std::unordered_map<size_t, size_t> &ranks) const;

    std::unordered_map<size_t, size_t> filterFour(const std::unordered_map<size_t, size_t> &ranks) const;

    bool isContainsTarget(const std::vector<size_t> &temp) const;

    bool isContainsBombs(const std::vector<size_t> &hands) const;

    bool canSplit3(const std::unordered_map<size_t, size_t> &others) const;

    std::vector<std::vector<size_t>> combination(const std::vector<size_t> &n, ssize_t k) const;

    void withKicker(std::vector<std::vector<size_t>> &ret,
                    const std::vector<size_t> &       combination,
                    const std::vector<size_t> &       primal,
                    ssize_t                           kicker) const;

    void withKickerContainsTarget(std::vector<std::vector<size_t>> &ret,
                                  const std::vector<size_t> &       combination,
                                  const std::vector<size_t> &       primal,
                                  ssize_t                           kicker) const;

    std::vector<size_t> restoreHands(const std::vector<size_t> &          ret,
                                     const std::multimap<size_t, size_t> &ranksMultimap) const;

    std::vector<std::vector<size_t>> restoreHands(const std::vector<std::vector<size_t>> &ret,
                                                  const std::multimap<size_t, size_t> &   ranksMultimap) const;

    bool isSame(const std::unordered_map<size_t, size_t> &ranks, const std::string &category) const;

    bool isContinuous(const std::vector<size_t> &vector) const;

    bool isContinuous(size_t a_1, size_t a_n, ssize_t n) const;

    bool isChain(const std::unordered_map<size_t, size_t> &ranks) const;

    bool isPairChain(const std::unordered_map<size_t, size_t> &ranks) const;

    std::tuple<bool, HandsCategoryModel> isTrioChain(const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerate(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumeratePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateFour(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void enumeratePairChain(std::vector<std::vector<size_t>> &        ret,
                            const std::unordered_map<size_t, size_t> &ranks) const;

    void enumerateTrioChain(std::vector<std::vector<size_t>> &        ret,
                            const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveSolo(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustivePair(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrio(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioWithSolo(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioWithPair(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveChain(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustivePairChain(std::vector<std::vector<size_t>> &        ret,
                             const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioChain(std::vector<std::vector<size_t>> &        ret,
                             const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioChainWithSolo(std::vector<std::vector<size_t>> &        ret,
                                     const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveTrioChainWithPair(std::vector<std::vector<size_t>> &        ret,
                                     const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveFourWithSolo(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void exhaustiveFourWithPair(std::vector<std::vector<size_t>> &        ret,
                                const std::unordered_map<size_t, size_t> &ranks) const;

    void appendBombs(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

public:
    std::vector<size_t> intention(const std::vector<size_t> &hands, bool isStartingHand = false);

    std::vector<std::vector<size_t>> cardIntentions(const std::vector<size_t> &hands, bool isStartingHand = false);

    std::vector<size_t> hint(const std::vector<size_t> &hands);

    std::vector<std::vector<size_t>> cardHint(const std::vector<size_t> &hands);

    size_t getSplitCount(const std::vector<size_t> &hands, const std::unordered_map<size_t, size_t> &ranks) const;

    void sortHands(std::vector<std::vector<size_t>> &ret, const std::unordered_map<size_t, size_t> &ranks) const;

    void setTheHighestSingleCard(const std::vector<size_t> &                 hands,
                                 std::vector<std::vector<size_t>> &          vector,
                                 std::vector<std::vector<size_t>>::iterator &iterator);
};
PAGAMES_WINNER_POKER_END
#endif // PAGAMES_WINNER_JUDGE_H
