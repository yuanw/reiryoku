from yaml import load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

layer_dict = {
    "L0": "Base",
    "L1": "Fun",
    "L2": "Nav",
    "L3": "Med",
    "L4": "Num",
    "L5": "Pointer",
    "L6": "Sym"}

key_dict = {
    "LLAYER_POINTER": "Pointer",
    "LLAYER_MEDIA": "Me",
    "LLAYER_NAVIGATION": "Nav",
    "LLAYER_FUNCTION": "Fun",
    "LLAYER_NUMERAL": "Num",
    "LLAYER_SYMBOLS": "Sym",
    "ENT": "⏎",
    "LGUI": "◆",
    "RGUI": "◆",
    "LALT": "⎇",
# KC_RALT = AltGr
    "LSFT": "⇧",
     "RSFT": "⇧",
    "LCTL" : "⎈",
     "RCTL": "⎈",
    "LAG(1)":"⎇+◆+1",
    "LAG(2)":"⎇+◆+2",
}

new_data = {}
with open('reiryoku.yaml',  encoding="utf8") as reader:

    data = load(reader, Loader=Loader)
    # print(data)
    # print(data.keys())
    new_data['layout'] ={"qmk_keyboard": "bastardkb/charybdis/3x5/v2/splinky_3", "qmk_layout": "LAYOUT"}

    new_data['layers'] = {}
    # dict_keys(['layout', 'layers'])
    # print(data.get('layers').keys())
    for l in data.get('layers').keys():
        new_data['layers'][layer_dict[l]] = []
        for row in data.get('layers')[l]:
            new_row = []
            print(row)
            for k in row:
                if isinstance(k, str):
                    new_row.append(key_dict.get(k, k))
                elif isinstance(k, dict):
                    old_v = k.get('h','')
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
