#!/usr/bin/env python
# -*- coding: utf-8 -*-

import itertools

chiffre = "\x74\x5c\x17\x06\x24\x0a\x34\x0e\x24\x58\x43\x0f\x27\x5a\x06\x4a\x68\x7d\x44\x06\x68\x57\x16\x19\x21\x5b\x16\x18\x68\x40\x0c\x4b\x23\x5a\x0c\x1c\x68\x5c\x0c\x1c\x68\x4d\x0c\x1e\x6f\x42\x06\x4b\x3b\x5b\x0f\x1d\x2d\x50\x43\x02\x3c\x1a\x43\x21\x3d\x47\x17\x4b\x24\x51\x17\x4b\x25\x51\x43\x0a\x68\x45\x16\x02\x2b\x5f\x43\x0e\x25\x55\x0a\x07\x66\x14\x22\x03\x64\x14\x02\x05\x2c\x14\x2a\x4b\x2f\x41\x06\x18\x3b\x14\x1a\x04\x3d\x13\x11\x0e\x68\x5c\x06\x19\x2d\x14\x05\x04\x3a\x14\x17\x03\x2d\x14\x08\x0e\x31\x1a\x4d\x45\x68\x7c\x06\x19\x2d\x14\x0a\x1f\x68\x5d\x10\x51\x68\x51\x56\x58\x71\x0d\x51\x09\x2a\x01\x05\x5c\x29\x52\x55\x0d\x7d\x05\x54\x5c\x7b\x55\x07\x53\x2d\x55\x50\x5b\x7b\x07\x07\x5d\x7e\x1a\x43\x22\x2e\x14\x1a\x04\x3d\x14\x14\x04\x26\x50\x06\x19\x68\x43\x0b\x0a\x3c\x14\x0a\x18\x68\x5d\x17\x45\x66\x1a\x43\x1c\x2d\x58\x0f\x47\x68\x5e\x16\x18\x3c\x14\x02\x05\x68\x79\x27\x5e\x68\x5c\x02\x18\x20\x14\x0c\x0d\x68\x16\x14\x0e\x24\x58\x43\x0f\x27\x5a\x06\x49\x66\x14\x2b\x04\x38\x51\x43\x12\x27\x41\x43\x03\x29\x50\x43\x0d\x3d\x5a\x4d\x4b\x1a\x51\x0d\x0a\x3d\x50\x4d\x57\x67\x5c\x17\x06\x24\x0a"

MAX_CHARS = 4

def createPasswordGenerator():
	alphabet, length = '', MAX_CHARS
	for i in range(65,91):
                alphabet += chr(i)
        for i in range(97,123):
                alphabet += chr(i)
        for i in range(48,58):
                alphabet += chr(i)
	res = itertools.product(alphabet, repeat=length)
	for i in res: 
		yield ''.join(i)

def verifyHash(x):
	# avant tout, si la clé ne fait pas 4 char on /quit
	if len(x) != MAX_CHARS:
		return False
	# ensuite on calcule le hash
	h = 0
	for i in range(len(x)):
		h += ((ord(x[i]) - i + 33)^31025)
	return h == 124456

def dechiffre(cle):
	"""Renvoi le déchiffré obtenu à partir de la clé envoyée en argument"""
	retour = ''
	iCle = 0
	for i in chiffre:
		retour += chr(ord(i) ^ ord(cle[iCle]))
		iCle+=1
		if iCle >= len(cle):
			iCle = 0
	return retour

def clairEstClean(clair):
	"""Renvoi true si aucun caractère n'est interndit"""
	net = True
	allow = [0, 8, 9, 10, 11, 12, 13]
	deny = [i for i in range(256) if i < 32 or i > 128]
	for i in allow:
		deny.remove(i)
	for i in clair:
		if ord(i) in deny:
			net = False
	return net

def ratioAlpha(clair):
	"""Renvoi le coefficient de lettre min/maj et espace du message (entre 0 et 1)."""
	retour = 0
	for i in clair:
		if ord(i) == 32 or 65<=ord(i)<=90 or 97<=ord(i)<=122:
			retour += 1
	return float(retour)/len(clair)

if __name__ == '__main__':
	generator = createPasswordGenerator()
	for candidate in generator:
		if verifyHash(candidate):
			clair = dechiffre(candidate)
			if ratioAlpha(clair) >= 0.8:
				print('Le dechiffré semble assez clair avec le mot de passe %s, le voici :' % candidate)
				print(clair)
