// sans-serif 14px

#ifndef COMMON_JP_VARIABLE_16X16_SPRITE_FONT_H
#define COMMON_JP_VARIABLE_16X16_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_sprite_items_common_jp_variable_16x16.h"

namespace common
{

namespace jp
{

constexpr bn::string_view variable_16x16_sprite_font_utf8_characters[] = {
    "№", "─", "━", "│", "┃", "┄", "┅", "┆", "┇", "┈", "┉", "┊", "┋", "┌", "┍", "┎", "┏", "┐", "┑", "┒", "┓", "└", "┕", "┖", "┗", "┘", "┙", "┚", "┛", "├", "┝", "┞", "┟", "┠", "┡", "┢", "┣", "┤", "┥", "┦", "┧", "┨", "┩", "┪", "┫", "┬", "┭", "┮", "┯", "┰", "┱", "┲", "┳", "┴", "┵", "┶", "┷", "┸", "┹", "┺", "┻", "┼", "┽", "┾", "┿", "╀", "╁", "╂", "╃", "╄", "╅", "╆", "╇", "╈", "╉", "╊", "╋", "╌", "╍", "╎", "╏", "═", "║", "╒", "╓", "╔", "╕", "╖", "╗", "╘", "╙", "╚", "╛", "╜", "╝", "╞", "╟", "╠", "╡", "╢", "╣", "╤", "╥", "╦", "╧", "╨", "╩", "╪", "╫", "╬", "╭", "╮", "╯", "╰", "╱", "╲", "╳", "╴", "╵", "╶", "╷", "╸", "╹", "╺", "╻", "╼", "╽", "╾", "╿", "ぁ", "あ", "ぃ", "い", "ぅ", "う", "ぇ", "え", "ぉ", "お", "か", "が", "き", "ぎ", "く", "ぐ", "け", "げ", "こ", "ご", "さ", "ざ", "し", "じ", "す", "ず", "せ", "ぜ", "そ", "ぞ", "た", "だ", "ち", "ぢ", "っ", "つ", "づ", "て", "で", "と", "ど", "な", "に", "ぬ", "ね", "の", "は", "ば", "ぱ", "ひ", "び", "ぴ", "ふ", "ぶ", "ぷ", "へ", "べ", "ぺ", "ほ", "ぼ", "ぽ", "ま", "み", "む", "め", "も", "ゃ", "や", "ゅ", "ゆ", "ょ", "よ", "ら", "り", "る", "れ", "ろ", "ゎ", "わ", "ゐ", "ゑ", "を", "ん", "ゔ", "ゕ", "ゖ", "ァ", "ア", "ィ", "イ", "ゥ", "ウ", "ェ", "エ", "ォ", "オ", "カ", "ガ", "キ", "ギ", "ク", "グ", "ケ", "ゲ", "コ", "ゴ", "サ", "ザ", "シ", "ジ", "ス", "ズ", "セ", "ゼ", "ソ", "ゾ", "タ", "ダ", "チ", "ヂ", "ッ", "ツ", "ヅ", "テ", "デ", "ト", "ド", "ナ", "ニ", "ヌ", "ネ", "ノ", "ハ", "バ", "パ", "ヒ", "ビ", "ピ", "フ", "ブ", "プ", "ヘ", "ベ", "ペ", "ホ", "ボ", "ポ", "マ", "ミ", "ム", "メ", "モ", "ャ", "ヤ", "ュ", "ユ", "ョ", "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ヮ", "ワ", "ヰ", "ヱ", "ヲ", "ン", "ヴ", "ヵ", "ヶ", "ヷ", "ヸ", "ヹ", "ヺ", "一", "丁", "七", "万", "丈", "三", "上", "下", "不", "与", "且", "世", "丘", "丙", "両", "並", "中", "丸", "丹", "主", "久", "乏", "乗", "乙", "九", "乱", "乳", "乾", "了", "予", "争", "事", "二", "互", "五", "井", "亜", "亡", "交", "享", "京", "亭", "人", "仁", "今", "介", "仏", "仕", "他", "付", "仙", "代", "令", "以", "仮", "仰", "仲", "件", "任", "企", "伏", "伐", "休", "会", "伝", "伯", "伴", "伸", "伺", "似", "但", "位", "低", "住", "佐", "体", "何", "余", "作", "佳", "併", "使", "例", "侍", "供", "依", "価", "侮", "侯", "侵", "便", "係", "促", "俊", "俗", "保", "信", "修", "俳", "俵", "俸", "倉", "個", "倍", "倒", "候", "借", "倣", "値", "倫", "倹", "偉", "偏", "停", "健", "側", "偵", "偶", "偽", "傍", "傑", "傘", "備", "催", "債", "傷", "傾", "働", "像", "僕", "僚", "僧", "儀", "億", "儒", "償", "優", "元", "兄", "充", "兆", "先", "光", "克", "免", "児", "党", "入", "全", "八", "公", "六", "共", "兵", "具", "典", "兼", "内", "円", "冊", "再", "冒", "冗", "写", "冠", "冬", "冷", "准", "凍", "凝", "凡", "処", "凶", "凸", "凹", "出", "刀", "刃", "分", "切", "刈", "刊", "刑", "列", "初", "判", "別", "利", "到", "制", "刷", "券", "刺", "刻", "則", "削", "前", "剖", "剛", "剣", "剤", "副", "剰", "割", "創", "劇", "力", "功", "加", "劣", "助", "努", "励", "労", "効", "劾", "勅", "勇", "勉", "動", "勘", "務", "勝", "募", "勢", "勤", "勧", "勲", "勺", "匁", "包", "化", "北", "匠", "匹", "区", "医", "匿", "十", "千", "升", "午", "半", "卑", "卒", "卓", "協", "南", "単", "博", "占", "印", "危", "即", "却", "卵", "卸", "厄", "厘", "厚", "原", "厳", "去", "参", "又", "及", "友", "双", "反", "収", "叔", "取", "受", "叙", "口", "古", "句", "叫", "召", "可", "台", "史", "右", "号", "司", "各", "合", "吉", "同", "名", "后", "吏", "吐", "向", "君", "吟", "否", "含", "吸", "吹", "呈", "呉", "告", "周", "味", "呼", "命", "和", "咲", "哀", "品", "員", "哲", "唆", "唇", "唐", "唯", "唱", "商", "問", "啓", "善", "喚", "喜", "喝", "喪", "喫", "営", "嗣", "嘆", "嘱", "器", "噴", "嚇", "囚", "四", "回", "因", "団", "困", "囲", "図", "固", "国", "圏", "園", "土", "圧", "在", "地", "坂", "均", "坊", "坑", "坪", "垂", "型", "垣", "埋", "城", "域", "執", "培", "基", "堀", "堂", "堅", "堕", "堤", "堪", "報", "場", "塀", "塁", "塊", "塑", "塔", "塗", "塚", "塩", "塾", "境", "墓", "増", "墜", "墨", "墳", "墾", "壁", "壇", "壊", "壌", "士", "壮", "声", "壱", "売", "変", "夏", "夕", "外", "多", "夜", "夢", "大", "天", "太", "夫", "央", "失", "奇", "奉", "奏", "契", "奔", "奥", "奨", "奪", "奮", "女", "奴", "好", "如", "妃", "妄", "妊", "妙", "妥", "妨", "妹", "妻", "姉", "始", "姓", "委", "姫", "姻", "姿", "威", "娘", "娠", "娯", "婆", "婚", "婦", "婿", "媒", "嫁", "嫌", "嫡", "嬢", "子", "孔", "字", "存", "孝", "季", "孤", "学", "孫", "宅", "宇", "守", "安", "完", "宗", "官", "宙", "定", "宜", "宝", "実", "客", "宣", "室", "宮", "宰", "害", "宴", "宵", "家", "容", "宿", "寂", "寄", "密", "富", "寒", "寛", "寝", "察", "寡", "寧", "審", "寮", "寸", "寺", "対", "寿", "封", "専", "射", "将", "尉", "尊", "尋", "導", "小", "少", "尚", "就", "尺", "尼", "尽", "尾", "尿", "局", "居", "屈", "届", "屋", "展", "属", "層", "履", "屯", "山", "岐", "岩", "岬", "岳", "岸", "峠", "峡", "峰", "島", "崇", "崎", "崩", "川", "州", "巡", "巣", "工", "左", "巧", "巨", "差", "己", "巻", "市", "布", "帆", "希", "帝", "帥", "師", "席", "帯", "帰", "帳", "常", "帽", "幅", "幕", "幣", "干", "平", "年", "幸", "幹", "幻", "幼", "幽", "幾", "庁", "広", "床", "序", "底", "店", "府", "度", "座", "庫", "庭", "庶", "康", "庸", "廃", "廉", "廊", "延", "廷", "建", "弁", "弊", "式", "弐", "弓", "弔", "引", "弟", "弦", "弧", "弱", "張", "強", "弾", "当", "形", "彩", "彫", "彰", "影", "役", "彼", "往", "征", "径", "待", "律", "後", "徐", "徒", "従", "得", "御", "復", "循", "微", "徳", "徴", "徹", "心", "必", "忌", "忍", "志", "忘", "忙", "応", "忠", "快", "念", "怒", "怖", "思", "怠", "急", "性", "怪", "恋", "恐", "恒", "恥", "恨", "恩", "恭", "息", "恵", "悔", "悟", "悠", "患", "悦", "悩", "悪", "悲", "悼", "情", "惑", "惜", "惨", "惰", "想", "愁", "愉", "意", "愚", "愛", "感", "慈", "態", "慌", "慎", "慕", "慢", "慣", "慨", "慮", "慰", "慶", "憂", "憎", "憤", "憩", "憲", "憶", "憾", "懇", "懐", "懲", "懸", "成", "我", "戒", "戦", "戯", "戸", "戻", "房", "所", "扇", "扉", "手", "才", "打", "払", "扱", "扶", "批", "承", "技", "抄", "把", "抑", "投", "抗", "折", "抜", "択", "披", "抱", "抵", "抹", "押", "抽", "担", "拍", "拐", "拒", "拓", "拘", "拙", "招", "拝", "拠", "拡", "括", "拷", "拾", "持", "指", "挑", "挙", "挟", "振", "挿", "捕", "捜", "捨", "据", "掃", "授", "掌", "排", "掘", "掛", "採", "探", "接", "控", "推", "措", "掲", "描", "提", "揚", "換", "握", "揮", "援", "揺", "損", "搬", "搭", "携", "搾", "摂", "摘", "摩", "撃", "撤", "撮", "撲", "擁", "操", "擦", "擬", "支", "改", "攻", "放", "政", "故", "敏", "救", "敗", "教", "敢", "散", "敬", "数", "整", "敵", "敷", "文", "斉", "斎", "斗", "料", "斜", "斤", "斥", "断", "新", "方", "施", "旅", "旋", "族", "旗", "既", "日", "旧", "旨", "早", "旬", "昆", "昇", "明", "易", "昔", "星", "映", "春", "昨", "昭", "是", "昼", "時", "晩", "普", "景", "晴", "晶", "暁", "暇", "暑", "暖", "暗", "暦", "暫", "暮", "暴", "曇", "曜", "曲", "更", "書", "曹", "替", "最", "月", "有", "服", "朕", "朗", "望", "朝", "期", "木", "未", "末", "本", "札", "朱", "朴", "机", "朽", "杉", "材", "村", "束", "条", "来", "杯", "東", "松", "板", "析", "林", "枚", "果", "枝", "枠", "枢", "枯", "架", "柄", "某", "染", "柔", "柱", "柳", "査", "栄", "栓", "校", "株", "核", "根", "格", "栽", "桃", "案", "桑", "桜", "桟", "梅", "械", "棄", "棋", "棒", "棚", "棟", "森", "棺", "植", "検", "業", "極", "楼", "楽", "概", "構", "様", "槽", "標", "模", "権", "横", "樹", "橋", "機", "欄", "欠", "次", "欧", "欲", "欺", "款", "歌", "歓", "止", "正", "武", "歩", "歯", "歳", "歴", "死", "殉", "殊", "残", "殖", "殴", "段", "殺", "殻", "殿", "母", "毎", "毒", "比", "毛", "氏", "民", "気", "水", "氷", "永", "汁", "求", "汗", "汚", "江", "池", "決", "汽", "沈", "沖", "没", "沢", "河", "沸", "油", "治", "沼", "沿", "況", "泉", "泊", "泌", "法", "泡", "波", "泣", "泥", "注", "泰", "泳", "洋", "洗", "洞", "津", "洪", "活", "派", "流", "浄", "浅", "浜", "浦", "浪", "浮", "浴", "海", "浸", "消", "涙", "涯", "液", "涼", "淑", "淡", "深", "混", "添", "清", "渇", "済", "渉", "渋", "渓", "減", "渡", "渦", "温", "測", "港", "湖", "湯", "湾", "湿", "満", "源", "準", "溝", "溶", "滅", "滋", "滑", "滝", "滞", "滴", "漁", "漂", "漆", "漏", "演", "漠", "漢", "漫", "漬", "漸", "潔", "潜", "潟", "潤", "潮", "澄", "激", "濁", "濃", "濫", "濯", "瀬", "火", "灯", "灰", "災", "炉", "炊", "炎", "炭", "点", "為", "烈", "無", "焦", "然", "焼", "煙", "照", "煩", "煮", "熟", "熱", "燃", "燥", "爆", "爵", "父", "片", "版", "牛", "牧", "物", "牲", "特", "犠", "犬", "犯", "状", "狂", "狩", "独", "狭", "猛", "猟", "猫", "献", "猶", "猿", "獄", "獣", "獲", "玄", "率", "玉", "王", "珍", "珠", "班", "現", "球", "理", "琴", "環", "璽", "瓶", "甘", "甚", "生", "産", "用", "田", "由", "甲", "申", "男", "町", "画", "界", "畑", "畔", "留", "畜", "畝", "略", "番", "異", "畳", "疎", "疑", "疫", "疲", "疾", "病", "症", "痘", "痛", "痢", "痴", "療", "癒", "癖", "発", "登", "白", "百", "的", "皆", "皇", "皮", "皿", "盆", "益", "盗", "盛", "盟", "監", "盤", "目", "盲", "直", "相", "盾", "省", "看", "県", "真", "眠", "眺", "眼", "着", "睡", "督", "瞬", "矛", "矢", "知", "短", "矯", "石", "砂", "研", "砕", "砲", "破", "硝", "硫", "硬", "碁", "碑", "確", "磁", "磨", "礁", "礎", "示", "礼", "社", "祈", "祉", "祖", "祝", "神", "祥", "票", "祭", "禁", "禅", "禍", "福", "秀", "私", "秋", "科", "秒", "秘", "租", "秩", "称", "移", "程", "税", "稚", "種", "稲", "稼", "稿", "穀", "穂", "積", "穏", "穫", "穴", "究", "空", "突", "窃", "窒", "窓", "窮", "窯", "立", "竜", "章", "童", "端", "競", "竹", "笑", "笛", "符", "第", "筆", "等", "筋", "筒", "答", "策", "箇", "算", "管", "箱", "節", "範", "築", "篤", "簡", "簿", "籍", "米", "粉", "粋", "粒", "粗", "粘", "粛", "粧", "精", "糖", "糧", "糸", "系", "糾", "紀", "約", "紅", "紋", "納", "純", "紙", "級", "紛", "素", "紡", "索", "紫", "累", "細", "紳", "紹", "紺", "終", "組", "経", "結", "絞", "絡", "給", "統", "絵", "絶", "絹", "継", "続", "維", "綱", "網", "綿", "緊", "総", "緑", "緒", "線", "締", "編", "緩", "緯", "練", "縁", "縄", "縛", "縦", "縫", "縮", "績", "繁", "繊", "織", "繕", "繭", "繰", "缶", "罪", "置", "罰", "署", "罷", "羅", "羊", "美", "群", "義", "羽", "翁", "翌", "習", "翻", "翼", "老", "考", "者", "耐", "耕", "耗", "耳", "聖", "聞", "聴", "職", "肉", "肌", "肖", "肝", "肢", "肥", "肩", "肪", "肯", "育", "肺", "胃", "胆", "背", "胎", "胞", "胴", "胸", "能", "脂", "脅", "脈", "脚", "脱", "脳", "脹", "腐", "腕", "腰", "腸", "腹", "膚", "膜", "膨", "臓", "臣", "臨", "自", "臭", "至", "致", "興", "舌", "舎", "舗", "舞", "舟", "航", "般", "舶", "船", "艇", "艦", "良", "色", "芋", "芝", "花", "芳", "芸", "芽", "苗", "若", "苦", "英", "茂", "茎", "茶", "草", "荒", "荘", "荷", "菊", "菌", "菓", "菜", "華", "落", "葉", "著", "葬", "蒸", "蓄", "蔵", "薄", "薦", "薪", "薫", "薬", "藩", "藻", "虐", "虚", "虜", "虞", "虫", "蚊", "蚕", "蛇", "蛍", "蛮", "融", "血", "衆", "行", "術", "街", "衛", "衝", "衡", "衣", "表", "衰", "衷", "袋", "被", "裁", "裂", "装", "裏", "裕", "補", "裸", "製", "複", "褐", "褒", "襟", "襲", "西", "要", "覆", "覇", "見", "規", "視", "覚", "覧", "親", "観", "角", "解", "触", "言", "訂", "計", "討", "訓", "託", "記", "訟", "訪", "設", "許", "訳", "訴", "診", "証", "詐", "詔", "評", "詞", "詠", "試", "詩", "詰", "話", "該", "詳", "誇", "誉", "誌", "認", "誓", "誕", "誘", "語", "誠", "誤", "説", "読", "課", "調", "談", "請", "論", "諭", "諮", "諸", "諾", "謀", "謁", "謄", "謙", "講", "謝", "謡", "謹", "識", "譜", "警", "議", "譲", "護", "谷", "豆", "豊", "豚", "象", "豪", "貝", "貞", "負", "財", "貢", "貧", "貨", "販", "貫", "責", "貯", "貴", "買", "貸", "費", "貿", "賀", "賃", "賄", "資", "賊", "賓", "賛", "賜", "賞", "賠", "賢", "賦", "質", "購", "贈", "赤", "赦", "走", "赴", "起", "超", "越", "趣", "足", "距", "跡", "路", "跳", "践", "踊", "踏", "躍", "身", "車", "軌", "軍", "軒", "軟", "転", "軸", "軽", "較", "載", "輝", "輩", "輪", "輸", "轄", "辛", "辞", "辱", "農", "辺", "込", "迅", "迎", "近", "返", "迫", "迭", "述", "迷", "追", "退", "送", "逃", "逆", "透", "逐", "逓", "途", "通", "逝", "速", "造", "連", "逮", "週", "進", "逸", "遂", "遅", "遇", "遊", "運", "遍", "過", "道", "達", "違", "遠", "遣", "適", "遭", "遮", "遵", "遷", "選", "遺", "避", "還", "邦", "邪", "邸", "郊", "郎", "郡", "部", "郭", "郵", "郷", "都", "酌", "配", "酒", "酔", "酢", "酪", "酬", "酵", "酷", "酸", "醜", "醸", "釈", "里", "重", "野", "量", "金", "針", "釣", "鈍", "鈴", "鉄", "鉛", "鉢", "鉱", "銀", "銃", "銅", "銑", "銘", "銭", "鋭", "鋳", "鋼", "錘", "錠", "錬", "錯", "録", "鍛", "鎖", "鎮", "鏡", "鐘", "鑑", "長", "門", "閉", "開", "閑", "間", "関", "閣", "閥", "閲", "闘", "防", "阻", "附", "降", "限", "陛", "院", "陣", "除", "陥", "陪", "陰", "陳", "陵", "陶", "陸", "険", "陽", "隅", "隆", "隊", "階", "随", "隔", "際", "障", "隠", "隣", "隷", "隻", "雄", "雅", "集", "雇", "雌", "雑", "離", "難", "雨", "雪", "雰", "雲", "零", "雷", "電", "需", "震", "霊", "霜", "霧", "露", "青", "静", "非", "面", "革", "靴", "音", "韻", "響", "頂", "項", "順", "預", "頑", "頒", "領", "頭", "頻", "頼", "題", "額", "顔", "顕", "願", "類", "顧", "風", "飛", "食", "飢", "飯", "飲", "飼", "飽", "飾", "養", "餓", "館", "首", "香", "馬", "駄", "駅", "駆", "駐", "騎", "騒", "験", "騰", "驚", "骨", "髄", "高", "髪", "鬼", "魂", "魅", "魔", "魚", "鮮", "鯨", "鳥", "鳴", "鶏", "麗", "麦", "麻", "黄", "黒", "黙", "鼓", "鼻", "齢", "｡", "｢", "｣", "､", "･", "ｦ", "ｧ", "ｨ", "ｩ", "ｪ", "ｫ", "ｬ", "ｭ", "ｮ", "ｯ", "ｰ", "ｱ", "ｲ", "ｳ", "ｴ", "ｵ", "ｶ", "ｷ", "ｸ", "ｹ", "ｺ", "ｻ", "ｼ", "ｽ", "ｾ", "ｿ", "ﾀ", "ﾁ", "ﾂ", "ﾃ", "ﾄ", "ﾅ", "ﾆ", "ﾇ", "ﾈ", "ﾉ", "ﾊ", "ﾋ", "ﾌ", "ﾍ", "ﾎ", "ﾏ", "ﾐ", "ﾑ", "ﾒ", "ﾓ", "ﾔ", "ﾕ", "ﾖ", "ﾗ", "ﾘ", "ﾙ", "ﾚ", "ﾛ", "ﾜ", "ﾝ", "ﾞ", "ﾟ"
};

constexpr int8_t variable_16x16_sprite_font_character_widths[] = {
    5, 5, 7, 9, 9, 13, 13, 4, 5, 5, 7, 11, 4, 7, 4, 6, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 4, 4, 0, 11, 0, 7, 15, 10, 9, 10, 11, 8, 8, 11, 11, 5, 6, 9, 8, 14, 12, 12, 9, 12, 10, 9, 9, 11, 10, 15, 10, 9, 9, 5, 0, 5, 11, 7, 0, 8, 9, 8, 9, 8, 5, 9, 9, 4, 4, 8, 4, 14, 9, 9, 9, 9, 6, 7, 6, 9, 8, 12, 8, 8, 7, 5, 4, 5, 0, 16, 10, 9, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 9, 9, 9, 10, 9, 9, 9, 10, 9, 9, 9, 10, 9, 9, 9, 10, 9, 9, 9, 9, 9, 9, 9, 10, 9, 9, 9, 9, 9, 9, 9, 10, 9, 9, 9, 9, 9, 9, 9, 10, 9, 9, 9, 9, 9, 9, 9, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 14, 14, 14, 14, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

constexpr bn::sprite_font variable_16x16_sprite_font(
        bn::sprite_items::common_jp_variable_16x16,
        variable_16x16_sprite_font_utf8_characters,
        variable_16x16_sprite_font_character_widths);

}

}

#endif