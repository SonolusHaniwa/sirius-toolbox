struct System_String {
	int32_t stringLength;
	uint16_t firstChar;
};
struct AccessoryEffectMaster {
	int64_t Id;
	int64_t EffectMasterId;
	struct System_String Name;
	struct System_String Description;
};
struct AccessoryLevelPatternItemMaster {
	int64_t ItemMasterId;
	int32_t Quantity;
};
struct AccessoryLevelPatternMaster {
	int64_t Id;
	int64_t AccessoryLevelPatternGroupMasterId;
	int32_t Level;
	int32_t RequiredCoin;
	vector<struct AccessoryLevelPatternItemMaster> Items;
};
struct AccessoryLevelPatternGroupMaster {
	int64_t Id;
	vector<struct AccessoryLevelPatternMaster> Patterns;
};
struct System_Int64 {
	int64_t m_value;
};
struct AccessoryMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Rarity;
	int64_t AccessoryLevelPatternGroupId;
	vector<struct System_Int64> FixedAccessoryEffects;
	vector<struct System_Int64> RandomEffectGroups;
	struct System_String PronounceName;
	int32_t Series;
	int32_t MaxLevel;
};
struct System_Decimal {
	int32_t flags;
	int32_t hi;
	int32_t lo;
	int32_t mid;
	uint64_t ulomidLE;
};
struct AchivementRateRewardMaster {
	int32_t ThingType;
	int64_t ThingId;
	int32_t ThingQuantity;
	int32_t Difficulty;
	struct System_Decimal AchivementRate;
};
struct AdditionalRewardThingMaster {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct AdditionalRewardPackageMaster {
	int64_t Id;
	struct System_String Description;
	int64_t LiveSettingMasterId;
	vector<struct AdditionalRewardThingMaster> Rewards;
	bool Etcetera;
};
struct AlbumEffectMaster {
	int64_t Id;
	int32_t Level;
	struct System_String Description;
	int64_t EffectMasterId;
};
struct AlbumThemeMaster {
	int64_t Id;
	int32_t Order;
	struct System_String Name;
};
struct System_DateTime {
	uint64_t dateData;
};
struct AuditionMaster {
	int64_t Id;
	int64_t MusicMasterId;
	int32_t RecommendedCompany;
	bool CanSkip;
	int64_t SenseNotationMasterId;
	struct System_String MaxPhase;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
	int32_t VocalVersion;
};
struct System_Nullable_int {
	bool hasValue;
	int32_t value;
};
struct AuditionPhaseMaster {
	int64_t Id;
	int64_t AuditionasterId;
	uint8_t Phase;
	int32_t RecommendedPlayerRank;
	int32_t ClearScore;
	struct System_Nullable_int StarActCount;
	int64_t AuditionRewardPackageMasterId;
};
struct AuditionRewardThing {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct AuditionRewardPackageMaster {
	int64_t Id;
	vector<struct AuditionRewardThing> Rewards;
};
struct System_Nullable_WebLinkTypes {
	bool hasValue;
	int32_t value;
};
struct BannerMaster {
	int64_t Id;
	int32_t Order;
	struct System_String Name;
	int32_t JumpType;
	struct System_String JumpPageId;
	struct System_Nullable_WebLinkTypes WebLinkType;
	struct System_String LinkUrl;
	int32_t DeleteConditionType;
	struct System_String DeleteConditionValue;
	struct System_String ImagePath;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
};
struct BodyMotionMaster {
	int64_t Id;
	struct System_String MotionName;
};
struct BombMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Order;
	bool Hidden;
	bool IsDefault;
};
struct EffectOrderMaster {
	int32_t Order;
	int64_t EffectMasterId;
};
struct System_Nullable_BranchJudgeTypes {
	bool hasValue;
	int32_t value;
};
struct System_Nullable_long {
	bool hasValue;
	int64_t value;
};
struct BranchMaster {
	int64_t Id;
	int32_t Order;
	vector<struct EffectOrderMaster> BranchEffects;
	struct System_Nullable_BranchJudgeTypes JudgeType1;
	struct System_Nullable_long Parameter1;
	struct System_Nullable_BranchJudgeTypes JudgeType2;
	struct System_Nullable_long Parameter2;
};
struct CampaignMaster {
	int64_t Id;
	struct System_String Title;
	struct System_String Description;
	struct System_String IconImagePath;
	int32_t Order;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	int64_t ComebackCampaignMasterId;
};
struct ChangeBodyMotionMaster {
	int64_t Id;
	struct System_String BeforeMotionName;
	struct System_String AfterMotionName;
	float Second;
};
struct CharacterAwakeningItemMaster {
	int64_t Id;
	struct System_String UnknownVariable1;
	struct System_String UnknownVariable2;
	int32_t AwakeningPhase;
	int64_t ItemMasterId;
	int32_t RequiredQuantity;
};
struct CharacterAwakeningItemGroupMaster {
	int64_t Id;
	vector<struct CharacterAwakeningItemMaster> Items;
};
struct CharacterBaseMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	struct System_String School;
	struct System_Nullable_int Grade;
	struct System_Nullable_int BirthMonth;
	struct System_Nullable_int BirthDay;
	int32_t Height;
	struct System_String Hobby;
	int64_t CompanyMasterId;
	struct System_String NameRomanization;
	struct System_String SenseName;
	struct System_String SenseEffect;
	struct System_String CharacterVoice;
	struct System_String ProfileImageAssetId;
	struct System_Nullable_int Age;
	struct System_String FamilyNameRomanization;
	struct System_String FirstNameRomanization;
	struct System_String PronounceFamilyName;
	struct System_String PronounceFirstName;
	struct System_String FamilyName;
	struct System_String FirstName;
	struct System_String EvoSenseName;
	struct System_String EvoSenseEffect;
	int64_t DefaultCostumeMasterId;
};
struct CharacterBloomBonusMaster {
	struct System_String UnknownVariable1;
	int32_t BloomBonusType;
	struct System_String Description;
	int32_t Phase;
	struct System_String UnknownVariable2;
	int64_t EffectMasterId;
	struct System_String IconPath;
};
struct CharacterBloomRewardMaster {
	int32_t Phase;
	int64_t ThingId;
	int32_t ThingType;
	struct System_String UnknownVariable1;
	int32_t ThingQuantity;
};
struct CharacterBloomBonusGroupMaster {
	int64_t Id;
	vector<struct CharacterBloomBonusMaster> BloomBonuses;
	vector<struct CharacterBloomRewardMaster> BloomRewards;
};
struct CharacterBloomDetailMaster {
	int64_t CharacterMasterId;
	struct System_String Text;
	struct System_String VoicePath;
};
struct CharacterBloomItemMaster {
	int32_t Rarity;
	int32_t CurrentStage;
	int32_t RequiredPieceAmount;
};
struct CharacterEpisodeMaster {
	int64_t Id;
	int64_t CharacterMasterId;
	int64_t EpisodeMasterId;
	int32_t EpisodeOrder;
	int32_t RequiredCharacterLevel;
};
struct CharacterEpisodeRelationMaster {
	int64_t CharacterMasterid;
	vector<struct System_Int64> PreviousStoryMasterIds;
	vector<struct System_Int64> NextStoryMasterIds;
	vector<struct System_Int64> SideStoryCharacterMasterIds;
	vector<struct System_Int64> RelatedStoryMasterIds
};
struct CharacterEpisodeReleaseItemMaster {
	int64_t ItemMasterId;
	int32_t RequiredQuantity;
	int32_t Order;
};
struct CharacterEpisodeReleaseItemGroupMaster {
	int64_t Id;
	vector<struct CharacterEpisodeReleaseItemMaster> Items;
};
struct CharacterExperienceItemMaster {
	int64_t Id;
	int64_t ItemMasterId;
	int64_t AcquirableExperience;
	float AcquirableExperienceBonus;
};
struct CharacterKeyMissionMaster {
	int64_t Id;
	int32_t Level;
	struct System_String Color;
	int32_t GiveStarPoint;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
};
struct CharacterLessonScoreRewardMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	int32_t RequiredScore;
	int64_t LessonScoreGroupMasterId;
};
struct CharacterLevelMaster {
	int32_t Level;
	int64_t ExperienceToLevelUp;
	int32_t CharacterStatusLevel;
	struct System_DateTime StartDate;
};
struct SiriusApi_Shared_Status {
	int32_t Vocal;
	int32_t Expression;
	int32_t Concentration;
};
struct CharacterMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	struct CharacterBaseMaster CharacterBaseMaster;
	struct System_String Name;
	struct System_String Description;
	struct System_String AssetId;
	int32_t Rarity;
	int32_t Attribute;
	struct SiriusApi_Shared_Status MinLevelStatus;
	int64_t StarActMasterId;
	struct System_Nullable_long AwakenStarActMasterId;
	int64_t SenseMasterId;
	bool ForbidGenericItemBloom;
	int64_t BloomBonusGroupMasterId;
	int64_t SenseEnhanceItemGroupMasterId;
	int64_t FirstEpisodeReleaseItemGroupId;
	int64_t SecondEpisodeReleaseItemGroupId;
	struct System_Nullable_long CharacterAwakeningItemGroupMasterId;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
};
struct CharacterMissionCategoryLevelMaster {
	int64_t Id;
	int32_t CategoryType;
	int32_t Level;
	int32_t GiveStarPoint;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
};
struct System_Nullable_JumpTypes {
	bool hasValue;
	int32_t value;
};
struct CharacterMissionStageMaster {
	int64_t Id;
	int64_t CharacterMissionCategoryLevelMasterId;
	int64_t CharacterMissionMasterId;
	bool ExclusionNoSenseCharacter;
	int32_t Order;
	int32_t StageOrder;
	int32_t GoalCount;
};
struct CharacterMissionMaster {
	int64_t Id;
	struct System_String Title;
	struct System_Nullable_JumpTypes JumpType;
	struct System_Nullable_long JumpValue;
	vector<struct CharacterMissionStageMaster> Stages;
};
struct CharacterPieceMaster {
	int64_t ItemMasterId;
	int64_t CharacterMasterId;
	int32_t DugongRequiredAmount;
};
struct CharacterProfileRestrictionMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	int32_t ProfileRestrictionItem;
	int64_t UnlockConditionMasterId;
	int64_t ProfileUnlockValue;
};
struct CharacterSenseEnhanceItemMaster {
	int32_t CurrentLevel;
	int64_t ItemMasterId;
	int32_t RequiredQuantity;
};
struct CharacterSenseEnhanceItemGroupMaster {
	int64_t Id;
	vector<struct CharacterSenseEnhanceItemMaster> Items;
};
struct CharacterStarRankMaster {
	int32_t Rank;
	int64_t NextRankPoint;
	int32_t RequiredLessonScore;
	float StatusBonus;
};
struct CharacterStarRankRewardMaster {
	int32_t ThingType;
	int64_t ThingId;
	int32_t ThingQuantity;
};
struct CharacterStarRankRewardGroupMaster {
	int64_t Id;
	vector<struct CharacterStarRankRewardMaster> Rewards;
};
struct ComebackCampaignMaster {
	int64_t Id;
	int32_t MissionActiveDays;
	int32_t JewelShopItemActiveDays;
	int32_t LoginBonusActiveDays;
	int32_t BuffCampaignActiveDays;
};
struct ComboRewardMaster {
	int32_t ThingType;
	int64_t ThingId;
	int32_t ThingQuantity;
	int32_t Difficulty;
	int32_t ComboCountPercent;
};
struct ComicMaster {
	int64_t Id;
	struct System_String GroupName;
	vector<struct EpisodeThing> Episodes;
};
struct ConcertMaster {
	int64_t Id;
	struct int64_t UnknownVariable1;
	struct DateTime StartDate;
	struct DateTime EndDate;
};
struct ConcertStageMaster {
	int64_t Id;
	int64_t ConcertMasterId;
	int64_t MusicMasterId;
	int32_t VocalVersion;
	int64_t SenseNotationMasterId;
	struct System_String RecommendedCompany;
	int32_t RecommendedLeveld;
	int32_t ClearScore;
	vector<struct ConcertRewardMaster> Rewards;
};
struct ConcertRewardMaster {
	int32_t ThingType;
	int64_t ThingId;
	int32_t ThingQuantity;
};
struct EpisodeThing {
	int64_t Id;
	int64_t Order;
	struct System_String Title;
	struct System_String Body;
	struct System_String StartDate;
};
struct CompanyMaster {
	int64_t Id;
	struct System_String Name;
	int32_t Companies;
	struct System_String Description;
};
struct CostumeGroupMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Order;
	vector<struct System_Int64> CostumeMasterIds;
	struct System_Nullable_long CostumeWearableCharacterGroupMasterId;
	struct System_Nullable_int PerformanceGroupId;
	bool IsHairChange;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
};
struct CostumeMaster {
	int64_t Id;
	struct System_String Name;
	int32_t Order;
	bool IsDefault;
	int64_t CostumeGroupMasterId;
	struct System_String Description;
};
struct CostumeWearableCharacterGroupMaster {
	int64_t Id;
	vector<struct System_Int64> CharacterBaseMasterIds;
};
struct DecorationMaster {
	int64_t Id;
	int32_t Order;
	bool IsDefault;
	struct System_String Name;
	int32_t Category;
	int32_t WidhtPx;
	int32_t HeightPx;
	int32_t MinWidthPx;
	int32_t MaxWidthPx;
};
struct EffectConditionMaster {
	int32_t Condition;
	struct System_Nullable_long Value;
};
struct EffectDetailMaster {
	int32_t Level;
	float Value;
};
struct EffectDurationMaster {
	int32_t Level;
	float DurationSeconds;
};
struct EffectDurationGroupMaster {
	int64_t Id;
	vector<struct EffectDurationMaster> Durations;
};
struct EffectTriggerMaster {
	int32_t Trigger;
	struct System_Nullable_long Value;
};
struct EffectMaster {
	int64_t Id;
	int32_t Type;
	int32_t Range;
	int32_t CalculationType;
	vector<struct EffectDetailMaster> Details;
	vector<struct EffectConditionMaster> Conditions;
	int32_t DurationSecond;
	vector<struct EffectTriggerMaster> Triggers;
	int32_t FireTimingType;
};
struct StoryMaster {
	int64_t Id;
	int32_t Type;
	struct System_Nullable_long CompanyMasterId;
	struct System_Nullable_long EventMasterId;
	int32_t ChapterOrder;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
};
struct EpisodeReleaseCondition {
	int32_t ConditionType;
	struct System_Nullable_long Value1;
	struct System_Nullable_long Value2;
	struct System_Nullable_long Value3;
};
struct EpisodeMaster {
	int64_t Id;
	int64_t StoryMasterId;
	struct System_String Title;
	int32_t Order;
	int64_t EpisodeRewardPackageMasterId;
	vector<struct EpisodeReleaseCondition> Conditions;
	struct System_Nullable_long PreEpisodeMasterId;
};
struct EpisodeRewardThing {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
	bool IsOnlyStoryEventTerm;
	bool IsChapterAllReadReward;
};
struct EpisodeRewardPackageMaster {
	int64_t Id;
	vector<struct EpisodeRewardThing> Rewards;
};
struct System_Nullable_DateTime {
	bool hasValue;
	struct System_DateTime value;
};
struct ItemMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int64_t DisplayOrder;
	struct System_Nullable_DateTime DisplayEndDate;
	int32_t MaxStock;
	int32_t Category;
	bool Consumable;
	struct System_Nullable_JumpTypes JumpType;
	struct System_Nullable_long JumpTargetId;
	int32_t TabCategory;
	int32_t Rarity;
};
struct System_Nullable_ShopReplaceTypes {
	bool hasValue;
	int32_t value;
};
struct System_Nullable_ShopUnlockTypes {
	bool hasValue;
	int32_t value;
};
struct ExchangeShopThing {
	int64_t Id;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
	struct System_Nullable_int ExchangeLimit;
	int32_t Order;
	struct System_Nullable_ShopReplaceTypes ReplaceType;
	bool IsDisplayLocked;
	struct System_Nullable_ShopUnlockTypes UnlockType;
	struct System_Nullable_long UnlockValue;
	struct System_Nullable_DateTime StartDate;
	struct System_Nullable_DateTime EndDate;
	int64_t RequiredItemMasterId;
	int32_t RequiredQuantity;
};
struct ExchangeShopMaster {
	int64_t Id;
	bool IsDisplayRequiredHavingItem;
	int32_t Category;
	struct System_String Name;
	int32_t DisplayThingType;
	struct System_Nullable_long DisplayItemMasterId;
	struct System_String BannerPath;
	struct System_Nullable_DateTime StartDate;
	struct System_Nullable_DateTime EndDate;
	struct System_DateTime LastRefreshedAt;
	vector<struct ExchangeShopThing> Lineup;
};
struct FacialExpressionMaster {
	int64_t Id;
	struct System_String EyeBrow;
	struct System_String Eye;
	struct System_String EyeBlink;
	struct System_String Cheek;
	struct System_String Mouth;
};
struct FilmItemMaster {
	int64_t Id;
	int64_t ItemMasterId;
	struct System_Nullable_long TargetCharacterBaseMasterId;
};
struct GachaDetailBonusThing {
	int64_t Id;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct GachaDetailMaster {
	int64_t Id;
	struct System_Nullable_long RequiredTicketMItemId;
	struct System_Nullable_int RequiredTicketQuantity;
	struct System_Nullable_int FreeJewelAmount;
	struct System_Nullable_int PaidJewelAmount;
	bool IsFree;
	struct System_Nullable_int DailyRollLimit;
	struct System_Nullable_int OverallRollLimit;
	int32_t PrizeCount;
	int32_t FixedPrizeCount;
	int32_t ButtonType;
	vector<struct GachaDetailBonusThing> DetailBonusThings;
};
struct GachaBonusThing {
	int64_t Id;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct GachaThing {
	int64_t Id;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
	struct System_Nullable_int PickupOrder;
};
struct GachaMaster {
	int64_t Id;
	struct System_String Name;
	int32_t CardType;
	int32_t GachaType;
	int64_t GachaTextTemplateMasterId;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	vector<struct GachaBonusThing> BonusThings;
	vector<struct GachaDetailMaster> GachaDetails;
	vector<struct GachaThing> Things;
	bool HasMovie;
	struct System_Nullable_long StoryEventMasterId;
	int64_t AttentionGachaTextTemplateMasterId;
	bool IsForceDisplay;
	bool IsHideEndDate;
};
struct GachaTextTemplateMaster {
	int64_t Id;
	struct System_String Description;
};
struct GameHintMaster {
	int64_t Id;
	int32_t PageCategory;
	int32_t PageCount;
};
struct GradualMissionMaster {
	int64_t Days;
	struct System_String UnknownVariable1;
	vector<struct System_Int64> MissionMasterIds;
};
struct GradualMissionGroupMaster {
	int64_t Id;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	vector<struct GradualMissionMaster> Missions;
};
struct HeadDirectionMaster {
	int64_t Id;
	struct System_String DirectionName;
};
struct HeadMotionMaster {
	int64_t Id;
	struct System_String MotionName;
};
struct HomeCharacterVoiceMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	struct System_String Text;
	int32_t Weight;
	struct System_String UnknownVariable1;
	struct System_Nullable_long CharacterVoicePeriodMasterId;
	struct System_String UnknownVariable2;
	bool IsPlayerBirthDateVoice;
	struct System_String VoiceFileName1;
	struct System_String VoiceFileName2;
	struct System_String VoiceFileName3;
	struct System_String VoiceFileName4;
	struct System_String UnknownVariable1;
	float VoiceInterval1;
	float VoiceInterval2;
	float VoiceInterval3;
	struct System_String MouthMotionId1;
	struct System_String MouthMotionId2;
	struct System_String MouthMotionId3;
	struct System_String MouthMotionId4;
	struct System_String BodyMotionId1;
	struct System_String BodyMotionId2;
	struct System_String BodyMotionId3;
	struct System_String BodyMotionId4;
};
struct System_TimeSpan {
	int64_t ticks;
};
struct HomeCharacterVoicePeriodMaster {
	int64_t Id;
	int32_t StartMonth;
	int32_t StartDay;
	struct System_TimeSpan StartTime;
	int32_t EndMonth;
	int32_t EndDay;
	struct System_TimeSpan EndTime;
};
struct HomePosterMaster {
	int64_t Id;
	int64_t PosterMasterId;
	struct System_Nullable_long GachaMasterId;
	struct System_DateTime DisplayStartDate;
	struct System_DateTime DisplayEndDate;
};
struct JewelShopCategoryMaster {
	int64_t Id;
	struct System_String Name;
	int32_t LayoutType;
	struct System_Nullable_int IsDisplayLocking;
	struct System_String BannerPath;
	int32_t Order;
};
struct System_Nullable_SaleTypes {
	bool hasValue;
	int32_t value;
};
struct System_Nullable_JewelShopUnlockTypes {
	bool hasValue;
	int32_t value;
};
struct JewelShopThing {
	int64_t Id;
	int64_t MJewelShopItemId;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct JewelShopItemMaster {
	int64_t Id;
	int32_t Order;
	struct System_String AppStoreProductId;
	struct System_String GooglePlayProductId;
	struct System_String Name;
	struct System_String PackText;
	int64_t MJewelShopCategoryId;
	int32_t DialogType;
	int32_t PurchaseType;
	struct System_Nullable_int PurchaseValue;
	bool SubscriptionFlag;
	struct System_Nullable_int GivePaidJewel;
	struct System_Nullable_ShopReplaceTypes ReplaceTypes;
	struct System_Nullable_int ReplaceValue;
	struct System_Nullable_int PurchaseLimit;
	struct System_Nullable_int ExpiredDay;
	struct System_String CanPurchaseDayOfWeek;
	bool IsBuyNotDisplay;
	struct System_Nullable_SaleTypes SaleType;
	struct System_Nullable_int SaleValue;
	bool IsDisplayLocking;
	struct System_Nullable_JewelShopUnlockTypes UnlockType;
	struct System_Nullable_long UnlockValue;
	struct System_Nullable_DateTime StartDate;
	struct System_Nullable_DateTime EndDate;
	vector<struct JewelShopThing> Lineup;
	int32_t ShopItemType;
	struct System_Nullable_long ShopItemValue;
	struct System_Nullable_long ComebackCampaignMasterId;
};
struct LeagueClassMaster {
	int64_t Id;
	struct System_Nullable_int UpUserAmount;
	struct System_Nullable_int KeepUserAmount;
	bool IsForceDownNotPlayingLeague;
	struct System_Nullable_long UpRewardPackageMasterId;
	struct System_Nullable_long KeepRewardPackageMasterId;
	struct System_Nullable_long DownRewardPackageMasterId;
	struct System_Nullable_long FirstAchieveRewardPackageMasterId;
	struct System_Nullable_long NameplateMasterId;
	int32_t ClassType;
	int64_t LeagueClassGroupMasterId;
};
struct LeagueClassGroupMaster {
	int64_t Id;
	vector<struct LeagueClassMaster> Classes;
};
struct LeagueGroupRankingRewardThingMaster {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct LeagueGroupRankingRewardPackageMaster {
	int64_t Id;
	vector<struct LeagueGroupRankingRewardThingMaster> Rewards;
	int32_t ClassType;
	int32_t MinRank;
	int32_t MaxRank;
	struct System_String RewardTitle;
};
struct LeagueMaster {
	int64_t Id;
	int64_t MusicMasterId;
	int32_t MaxDifficulty;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime CountingStartAt;
	struct System_DateTime DisplayEndAt;
	int64_t LeagueClassGroupMasterId;
	int64_t SenseNotationMasterId;
	int32_t VocalVersion;
};
struct LeaguePlayRewardThingMaster {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct LeaguePlayRewardPackageMaster {
	int64_t Id;
	vector<struct LeaguePlayRewardThingMaster> Rewards;
};
struct LeagueRewardThingMaster {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct LeagueRewardPackageMaster {
	int64_t Id;
	vector<struct LeagueRewardThingMaster> Rewards;
};
struct LessonScoreRewardMaster {
	int64_t Id;
	int32_t ThingType;
	int64_t ThingId;
	int32_t ThingQuantity;
};
struct LessonScoreRewardGroupMaster {
	int64_t Id;
	vector<struct LessonScoreRewardMaster> Rewards;
};
struct LightLoadSplitEffectMaster {
	int64_t Id;
	int32_t BaseEffect;
	int32_t LightLoadEffect;
};
struct LipSyncMaster {
	int64_t Id;
	struct System_String MotionName;
};
struct LiveDropThingMaster {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
	bool IsSpecialFallCountUpThing;
	bool IsSpecialFallThing;
};
struct LiveDropFrameMaster {
	int64_t Id;
	int64_t LiveDropFrameGroupMasterId;
	int32_t Order;
	int32_t ChangeType;
	int32_t FrameLotCondition;
	bool UnaffectedIncreaseEffect;
	vector<struct LiveDropThingMaster> Rewards;
	struct System_Nullable_int FrameLotConditionValue;
};
struct LiveDropFrameGroupMaster {
	int64_t Id;
	vector<struct LiveDropFrameMaster> DropFrames;
};
struct LiveMaster {
	int64_t Id;
	int32_t Difficulty;
	int64_t MusicMasterId;
	int32_t Level;
};
struct LiveSettingMaster {
	int64_t Id;
	struct System_String Name;
	int32_t LiveType;
	int64_t LiveDropFrameGroupMasterId;
};
struct LoopMotionMaster {
	int64_t Id;
	struct System_String TargetCharacterBaseId;
	float LoopSpeed;
	int32_t Height;
	int32_t Size;
};
struct MarketFrameThingMaster {
	int64_t Id;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
	struct System_String UnknownVariable1;
	struct System_String UnknownVariable2;
	int32_t RequiredThingType;
	int64_t RequiredThingId;
	int32_t RequiredThingQuantity;
	int32_t DisplayType;
};
struct MissionRewardMaster {
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct MissionStageMaster {
	int64_t Id;
	int32_t MissionStageOrder;
	int32_t StageGoalValue;
	struct System_Nullable_DateTime StartDate;
	vector<struct MissionRewardMaster> Rewards;
};
struct MissionMaster {
	int64_t Id;
	int32_t MissionCategory;
	int32_t MissionViewOrder;
	struct System_String Title;
	struct System_String Description;
	struct System_String UnknownVariable1;
	struct System_String UnknownVariable2;
	struct System_String UnknownVariable3;
	struct System_String UnknownVariable4;
	struct System_String UnknownVariable5;
	struct System_String UnknownVariable6;
	struct System_Nullable_long EventMasterId;
	int32_t JumpType;
	struct System_Nullable_long JumpTargetId;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	vector<struct MissionStageMaster> Stages;
	struct System_Nullable_long ComebackCampaignMasterId;
};
struct MissionPassRewardThing {
	int64_t Id;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
	int64_t MissionPassDetailMasterId;
	bool IsSp;
};
struct MissionPassDetailMaster {
	int64_t Id;
	int32_t Phase;
	struct System_Nullable_long MissionPassMasterId;
	int32_t ClearPoint;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	vector<struct MissionPassRewardThing> Rewards;
};
struct MissionPassMaster {
	int64_t Id;
	int64_t ItemMasterId;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
};
struct MultiLiveScheduleMaster {
	int64_t Id;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	int64_t LiveSettingMasterId1;
	int64_t LiveSettingMasterId2;
};
struct MusicVocalVersionMaster {
	int64_t Id;
	int64_t MusicMasterId;
	int32_t VocalVersion;
	struct System_String Singer;
	struct System_String Name;
	int32_t MusicTimeSecond;
	float SampleStartSeconds;
	float SampleEndSeconds;
	int32_t MusicVideoType;
	vector<struct System_Int64> Characters;
};
struct MusicMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	struct System_String UnknownVariable1;
	int64_t RewardRuleMasterId;
	struct System_String PronounceName;
	struct System_String LyricWriter;
	struct System_String Composer;
	struct System_String Arranger;
	struct System_String UnknownVariable2;
	struct System_String UnlockText;
	bool IsLongVersion;
	struct System_Nullable_DateTime ReleasedAt;
	int32_t StaminaConsumption;
	int32_t MusicTimeSecond;
	bool Invisible;
	struct System_String UnknownVariable3;
	float SampleStartSeconds;
	float SampleEndSeconds;
	float DelaySeconds;
	vector<struct MusicVocalVersionMaster> VocalVersions;
	int32_t UnlockConditionType;
	struct System_Nullable_long UnlockConditionValue;
	int32_t MusicVideoType;
	int32_t MusicCoverType;
};
struct MusicVideoDefaultCostumeMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	int64_t CostumeMasterId;
};
struct MusicVideoDefaultCostumeGroupMaster {
	int64_t Id;
	vector<struct MusicVideoDefaultCostumeMaster> DefaultCostumes;
};
struct MusicVideoOriginalCharacterMaster {
	int32_t Order;
	int64_t CharacterBaseMasterId;
	int64_t CostumeMasterId;
};
struct MusicVideoMaster {
	int64_t Id;
	int64_t MusicMasterId;
	int32_t MemberAmount;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
	vector<struct MusicVideoOriginalCharacterMaster> Characters;
	float DelaySeconds;
	float MovieDelaySeconds;
	struct System_Nullable_long MusicVideoDefaultCostumeGroupMasterId;
};
struct NameColorMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Order;
	bool Hidden;
	bool IsDefault;
	struct System_String UnlockText;
};
struct NameplateDetailMaster {
	int64_t Id;
	struct System_String Name;
	int32_t ClassType;
	int32_t EnrollCount;
};
struct NameplateMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Order;
	bool Hidden;
	bool IsDefault;
	vector<struct NameplateDetailMaster> Details;
	struct System_String UnlockText;
};
struct NgWordMaster {
	int64_t Id;
	struct System_String KeyWord;
};
struct NoteMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Order;
	bool Hidden;
	bool IsDefault;
};
struct PhotoEffectMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int64_t EffectMasterId;
	struct System_Nullable_int Variety;
};
struct PhotoLevelUpItemMaster {
	int64_t ItemMasterId;
	int32_t RequiredQuantity;
};
struct PhotoLevelUpItemGroupMaster {
	int64_t Id;
	int32_t Rarity;
	int32_t BaseLevel;
	int32_t RequiredCoin;
	vector<struct PhotoLevelUpItemMaster> Items;
};
struct PhotoSpotMaster {
	int64_t Id;
	struct System_String Name;
	int32_t ReleasePlayerRank;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
};
struct PlayerRankMaster {
	int32_t Rank;
	int32_t PointToLevelUp;
	int32_t MaxStamina;
	struct System_String UnknownVariable1;
	bool IsReleasedRank;
};
struct PosterAbilityMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int64_t PosterMasterId;
	int32_t Type;
	struct System_String UnknownVariable1;
	int32_t FrameNumber;
	int32_t ReleaseLevelAt;
	bool Hidden;
	vector<struct BranchMaster> Branches;
	int32_t BranchConditionType1;
	struct System_Nullable_long ConditionValue1;
	int32_t BranchConditionType2;
	struct System_Nullable_long ConditionValue2;
};
struct PosterCharacterMaster {
	int32_t EpisodeType;
	struct System_Nullable_long CharacterBaseMasterId;
	struct System_String Description;
};
struct PosterCostumeMaster {
	int32_t Phase;
	int64_t CostumeMasterId;
};
struct PosterLevelPatternMaster {
	int64_t Id;
	int64_t LevelPatternGroupId;
	int32_t Level;
	int64_t ItemMasterId;
	int32_t Quantity;
};
struct PosterLevelPatternGroupMaster {
	int64_t Id;
	vector<struct PosterLevelPatternMaster> Patterns;
};
struct System_Nullable_float {
	bool hasValue;
	float value;
};
struct PosterMaster {
	int64_t Id;
	struct System_String Name;
	struct System_Nullable_int OrganizeRestrictGroupId;
	int32_t Rarity;
	struct System_String UnknownVariable1;
	int64_t LevelPatternGroupMasterId;
	struct System_Nullable_float SubTitlePositionX1;
	struct System_Nullable_float SubTitlePositionY1;
	struct System_Nullable_float SubTitlePositionX2;
	struct System_Nullable_float SubTitlePositionY2;
	struct System_Nullable_float SubTitlePositionX3;
	struct System_Nullable_float SubTitlePositionY3;
	int64_t ReleaseItemGroupId;
	struct System_String UnknownVariable2;
	struct System_String UnknownVariable3;
	struct System_String PronounceName;
	vector<struct PosterCostumeMaster> Costumes;
	vector<struct System_Int64> AppearanceCharacterBaseMasterIds;
	bool IsRestrictItemBreakThrough;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
};
struct PosterReleaseItemMaster {
	int32_t CurrentPhase;
	int64_t ItemMasterId;
	int32_t RequiredQuantity;
};
struct PosterReleaseItemGroupMaster {
	int64_t Id;
	vector<struct PosterReleaseItemMaster> Items;
};
struct PosterStoryMaster {
	int64_t Id;
	int64_t PosterMasterId;
	int32_t EpisodeType;
	struct System_Nullable_long CharacterBaseMasterId;
	struct System_String Description;
	int32_t Order;
};
struct RandomEffectGroupMaster {
	int64_t Id;
	vector<struct System_Int64> AccessoryEffects;
};
struct ResultVoiceMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	int32_t Order;
	int32_t ConditionType;
	struct System_String VoiceAssetId;
	struct System_String Motion;
};
struct RewardRuleMaster {
	int64_t Id;
	vector<struct AchivementRateRewardMaster> AchivementRateRewards;
};
struct SceneCameraMaster {
	int64_t Id;
	int32_t StartPositionX;
	int32_t StartPositionY;
	int32_t StartZoomRatio;
	int32_t EndPositionX;
	int32_t EndPositionY;
	int32_t EndZoomRatio;
	int32_t CameraMoveTurnaroundTimeSeconds;
};
struct ScoreRewardMaster {
	int32_t ThingType;
	int64_t ThingId;
	int32_t ThingQuantity;
};
struct SenseEffectMaster {
	int32_t Order;
	int64_t EffectMasterId;
};
struct SenseBranchMaster {
	int32_t Order;
	vector<struct SenseEffectMaster> Effects;
};
struct SenseMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Type;
	vector<struct EffectOrderMaster> PreEffects;
	vector<struct BranchMaster> Branches;
	int32_t AcquirableGauge;
	int32_t AcquirableScorePercent;
	int32_t ScoreUpPerLevel;
	int32_t LightCount;
	int32_t CoolTime;
};
struct SenseNotationBuffMaster {
	int64_t Id;
	int32_t Type;
	struct System_Nullable_long TargetValue;
	int32_t BuffValue;
};
struct SenseNotationDetailMaster {
	int64_t Id;
	int32_t Position;
	int32_t TimingSecond;
};
struct SenseNotationMaster {
	int64_t Id;
	vector<struct SenseNotationDetailMaster> Details;
	vector<struct SenseNotationBuffMaster> Buffs;
};
struct SensePerformanceCharacterMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	int64_t SensePerformanceMasterId;
	struct System_String ExpressId;
};
struct SensePerformanceMaster {
	int64_t Id;
	int32_t LabelType;
	struct System_Nullable_int PerformanceGroup;
	vector<struct SensePerformanceCharacterMaster> Characters;
};
struct SignMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	struct System_String SignImagePath;
};
struct SpotConversationMaster {
	int64_t Id;
	int32_t Spot;
	struct System_Nullable_long CharacterId1;
	struct System_Nullable_long CharacterId2;
	struct System_Nullable_long CharacterId3;
	struct System_Nullable_long CharacterId4;
	struct System_Nullable_long CharacterId5;
	struct System_String UnknownVariable1;
	struct System_String UnknownVariable2;
	int64_t EpisodeMasterId;
	struct System_Nullable_long CostumeId1;
	struct System_Nullable_long CostumeId2;
	struct System_Nullable_long CostumeId3;
	struct System_Nullable_long CostumeId4;
	struct System_Nullable_long CostumeId5;
};
struct StaminaRecoveryItemMaster {
	int64_t ItemMasterId;
	int32_t RecoveryAmount;
	struct System_Nullable_DateTime ExpiredAt;
};
struct StampMaster {
	int64_t Id;
	int32_t Order;
	bool IsDefault;
	int64_t CharacterBaseMasterId;
	int32_t Type;
	struct System_String AssetId;
	struct System_String VoiceAssetId;
	struct System_String Name;
};
struct StarActConditionMaster {
	int64_t Id;
	int32_t FreeLight;
	int32_t SupportLight;
	int32_t ControlLight;
	int32_t AmplificationLight;
	int32_t SpecialLight;
};
struct StarActMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	struct System_String UnknownVariable1;
	int64_t StarActConditionMasterId;
	int32_t AcquirableScorePercent;
	int32_t ScoreUpPerLevel;
	vector<struct EffectOrderMaster> PreEffects;
	vector<struct BranchMaster> Branches;
};
struct StarRankRewardMaster {
	int32_t Rank;
	int64_t CharacterBaseMasterId;
	int64_t CharacterStarRankRewardGroupMasterId;
};
struct StoryEventBonusCharacterBaseMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int32_t BonusTargetType;
	int64_t BonusTargetId;
};
struct StoryEventCircleHighScoreRewardMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int32_t Score;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct StoryEventCircleMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int64_t MusicMasterId;
};
struct StoryEventCircleMissionMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	struct System_String Title;
	struct System_String CountUnit;
	int32_t GoalCount;
	int32_t MaxPhase;
	int32_t GivePointAtGoal;
};
struct StoryEventCircleMissionRewardMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int32_t RequiredCirclePoint;
	int32_t RequiredUserPoint;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct StoryEventEpisodeMaster {
	int64_t Id;
	int64_t StoryMasterId;
	struct System_String UnknownVariable1;
	int64_t RequiredTotalAcquiredPoint;
	struct System_String Title;
	int32_t Order;
	struct System_Nullable_long RequiredReadEpisodeMasterId;
	int64_t EpisodeRewardPackageMasterId;
};
struct StoryEventHighScoreBuffMaster {
	int64_t Id;
	int64_t StoryEventHighScoreBuffPatternGroupId;
	int64_t EffectMasterId;
	struct System_String EffectName;
	struct System_String EffectDescription;
	struct System_String IconImagePath;
};
struct StoryEventHighScoreBuffPatternMaster {
	int32_t NextLevel;
	int32_t RequiredPoint;
};
struct StoryEventHighScoreBuffPatternGroupMaster {
	int64_t Id;
	vector<struct StoryEventHighScoreBuffPatternMaster> Patterns;
};
struct StoryEventHighScoreBuffSettingMaster {
	int64_t Id;
	int64_t StoryEventHighScoreMasterId;
	int64_t StoryEventHighScoreBuffMasterId;
	struct StoryEventHighScoreBuffMaster StoryEventHighScoreBuffMaster;
};
struct StoryEventHighScoreMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int64_t MusicMasterId;
	vector<struct System_Int64> BuffIds;
	int64_t SenseNotationMasterId;
};
struct StoryEventHighScoreRewardMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int32_t Score;
	int64_t ThingId;
	int32_t ThingType;
	int32_t ThingQuantity;
};
struct StoryEventMaster {
	int64_t Id;
	int32_t Category;
	int32_t BonusAttribute;
	struct System_String Title;
	int64_t ExchangeShopMasterId;
	int64_t EventPointItemMasterId;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	struct System_DateTime ForceEndDate;
};
struct StoryEventRewardItemPackage {
	int64_t Id;
	vector<struct StoryEventCircleMaster> Rewards;
};
struct StoryEventRewardMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int32_t TargetMinRank;
	int32_t TargetMaxRank;
	int64_t StoryEventRewardItemPackageId;
};
struct StoryEventStoryMaster {
	int64_t Id;
	int64_t StoryMasterId;
	int64_t StoryEventMasterId;
	struct System_DateTime ListedStartDate;
	int32_t EventStoryListFlags;
	bool IsKeyStory;
};
struct StoryEventTotalPointRewardMaster {
	int64_t Id;
	int64_t StoryEventMasterId;
	int32_t RequiredTotalPoint;
	int32_t ThingType;
	int64_t ThingId;
	int32_t ThingQuantity;
};
struct StoryRelationMaster {
	int64_t StoryMasterId;
	vector<struct System_Int64> PreviousStoryMasterIds;
	vector<struct System_Int64> NextStoryMasterIds;
	vector<struct System_Int64> SideStoryCharacterMasterIds;
	vector<struct System_Int64> RelatedStoryMasterIds;
};
struct TheaterRoleMaster {
	struct System_String RoleName;
	int64_t CharacterBaseMasterId;
	int64_t CostumeMasterId;
	int32_t Order;
};
struct TheaterDetailMaster {
	int64_t Id;
	struct System_String Speaker;
	struct System_String Phrase;
	struct System_String VoiceAssetId;
};
struct TheaterStoryMaster {
	int64_t Id;
	struct System_String Name;
	int32_t Order;
	bool HasSynopsis;
	struct System_String Synopsis;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
	vector<struct TheaterRoleMaster> Roles;
	vector<struct TheaterDetailMaster> Details;
};
struct TheaterChapterMaster {
	int64_t Id;
	struct System_String Name;
	int64_t CompanyMasterId;
	struct System_DateTime DisplayStartAt;
	struct System_DateTime DisplayEndAt;
	vector<struct TheaterStoryMaster> Stories;
	struct System_Nullable_long EpisodeMasterId;
	int64_t RequiredReadEpisodeMasterId;
	vector<struct System_Int64> TheaterStoryMasterIds;
};
struct TipMaster {
	int64_t Id;
	int64_t CharacterBaseMasterId;
	struct System_String Description;
	struct System_String Name;
};
struct TournamentDetailMaster {
	int64_t Id;
	int64_t MusicMasterId;
	int32_t Difficulty;
};
struct TournamentMaster {
	int64_t Id;
	struct System_DateTime StartDate;
	struct System_DateTime EndDate;
	vector<struct TournamentDetailMaster> Details;
};
struct TrophyGroupMaster {
	int64_t Id;
	int32_t Category;
};
struct TrophyMaster {
	int64_t Id;
	struct System_String Name;
	struct System_String Description;
	int32_t Rarity;
	int32_t Order;
	int64_t TrophyGroupMasterId;
	bool Hidden;
	struct System_String UnlockText;
};
struct UnlockConditionMaster {
	int64_t Id;
	struct System_String ConditionText;
	int32_t ConditionUsableTableFilter;
	int32_t UnlockConditionType;
};
