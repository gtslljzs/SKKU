import os, re, sys, random, urllib.parse
from collections import defaultdict

def load_csv(fn):
    ret = []

    with open(fn, encoding='utf-8') as fin:
        for line in fin:
            lln = line.rstrip('\r\n').split('\t')
            ret.append(lln)

    return ret

def load_csvg(fn):
    with open(fn, encoding='utf-8') as fin:
        for line in fin:
            lln = line.rstrip('\r\n').split('\t')
            yield lln

def load_list(fn):
    with open(fn, encoding='utf-8') as fin:
        st = list(ll for ll in fin.read().split('\n') if ll != "")

    return st

def save_list(st, ofn):
    with open(ofn, 'w', encoding='utf-8') as fout:
        for k in st:
            fout.write(str(k) + "\n")


def freq_dict2list(dt):
    return sorted(dt.items(), key=lambda d:d[-1], reverse=True)
