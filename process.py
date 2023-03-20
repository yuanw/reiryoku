from yaml import load, dump
from collections import namedtuple
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

Point = namedtuple('Point', ['layer', 'row', 'key'])
# hardcode layer list
layer_dict = {
    "L0": "0:Base",
    "L1": "1:Fun",
    "L2": "2:Nav",
    "L3": "3:Med",
    "L4": "4:Num",
    "L5": "6:Pointer",
    "L6": "5:Sym"}

layer_key_dict = {
    "LLAYER_POINTER": "Pointer",
    "LLAYER_MEDIA": "Med",
    "LLAYER_NAVIGATION": "Nav",
    "LLAYER_FUNCTION": "Fun",
    "LLAYER_NUMERAL": "Num",
    "LLAYER_SYMBOLS": "Sym",
}

key_dict = {
    "LLAYER_POINTER": "Pointer",
    "LLAYER_MEDIA": "Med",
    "LLAYER_NAVIGATION": "Nav",
    "LLAYER_FUNCTION": "Fun",
    "LLAYER_NUMERAL": "Num",
    "LLAYER_SYMBOLS": "Sym",
    ",":  {"t": ",","s": "<"},
    ".": {"t":".", "s": ">"},
    ";": {"t":";", "s": ":"},
    # "ESC": "\u241B",
    "BSPC": "\u232B",
    "VOLD": "🔉",
    "VOLU": "🔊",
    "MUTE": "\U0001F507",
    "MNXT": "\u21E5",
    "MPRV": "\u21E4",
    "LEFT": "\u25C1",
    "RGHT": "\u25B7",
    "DOWN": "\u25BD",
    "REPEAT": "\u21BB",
    "UP": "\u25B3",
    "ENT": "⏎",
    "LGUI": "⌘",
    "RGUI": "⌘",
    "LALT": "⌥",
    "LSFT": "⇧",
     "RSFT": "⇧",
    "LCTL" : "⌃",
     "RCTL": "⌃",
    "CAPS": "⇪",
    "QK_CAPS_WORD_TOGGLE": "Caps Word",
    "LAG(1)":"⌘⌥1",
    "LAG(2)":"⌘⌥2",
    "LAG(3)":"⌘⌥3",
    "LAG(4)":"⌘⌥4",
    "LSG(1)": "⌘⇧1",
    "LSG(2)": "⌘⇧2",
    "LSG(3)": "⌘⇧3",
    "LSG(4)": "⌘⇧4",
}

layer_hold_dict = {}
new_data = {}
with open('reiryoku.yaml',  encoding="utf8") as reader:
    data = load(reader, Loader=Loader)
    new_data['layout'] ={"qmk_keyboard": "bastardkb/charybdis/3x5/v2/splinky_3", "qmk_layout": "LAYOUT"}
    # hardcoded combos list
    new_data['combos'] = [
        {'p': [1, 2],
         'k':'?',
         'l':['0:Base']},
        {'p': [2, 3],
         'k':'Q',
         'l':['Base']},
          {'p': [6, 7],
         'k':'Z',
           'l':['0:Base']},
         {'p': [22, 23],
         'k':'B',
         'l':['0:Base']},
         {'p': [21, 22],
         'k':'X',
         'l':['0:Base']},
        {'p': [26, 27],
         'k':'J',
         'l':['0:Base']},
    ]
    new_data['layers'] = {}
    for l in data.get('layers').keys():
        new_data['layers'][layer_dict[l]] = []
        for row_counter, row in enumerate(data.get('layers')[l]):
            new_row = []
            for key_counter, k in enumerate(row):
                # add held for layer activation
                # since this is miryoku-inspired
                p = Point(layer_dict[l], row_counter, key_counter)
                if p in layer_hold_dict:
                    new_row.append({'type':'held'})
                elif isinstance(k, str):
                    new_row.append(key_dict.get(k, k))
                elif isinstance(k, dict):
                    old_v = k.get('h','')
                    # add layer hold entry, so we can add svg held class
                    # highlight correctly
                    if old_v in layer_key_dict:
                        p = Point(layer_key_dict[old_v], row_counter, key_counter)
                        layer_hold_dict[p] = layer_dict[l]

                    k['h'] = key_dict.get(old_v, old_v)
                    if 't' in k:
                        old_v = k.get('t','')
                        k['t'] = key_dict.get(old_v, old_v)
                    new_row.append(k)
                else:
                    new_row.append(k)
            new_data['layers'][layer_dict[l]].append(new_row)

    # print(type(data.get('layers').get('L0'))) #list
    # l0 = data.get('layers').get('L0')
    # for row in l0:
    #     print(type(row)) #list
    #     for k in row:
    #         print(type(k))
    #         print(k)
with open('output.yaml', "w", encoding="utf8") as writer:
    dump(new_data, writer)
