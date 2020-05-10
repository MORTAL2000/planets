/*
Copyright Minerva Predavec, 2019
transfercalculator.com
*/

/* Format of planetary data
	loPE: Longitude of Periapsis
	loAN: Longitude of Ascending Node
	e: Orbital Eccentricity
	a: Semi-major axis
	i: Orbital Inclination
	L: Mean Longitude at epoch
	gravParam: gravitational parameter
	colour: Colour of planetary marker
	trackColour: Colour of orbital track
	center: PLanetary body it is centered on
	rotation: rotation period in days
	rotEpoch: Initial rotation at epoch
	bumpMap: if the planet has a heightmap
	reflection: if the planet has a specular map
	normalMap: If the height map is a normalMap map and not a bump map
	axialTilt: Unit vector the north pole points towards
	satellite: If the planet is a satellite and not an actual planetary body (no surface mesh)
	model: If the planet has a specific model associated with it
	heightMapScale: The scale of the bump/normalMap map
	modelScale: The scale of the model
	
	viewingClass: Whether the body should be displayed given various settings
	mapClass: What mapping systems should be used to display (Defaults are it does have a surface, no normal or height map, and no model)
*/

var planets = {
	mercury: {
		loPE: 77.45645,
		loAN: 48.33167,
		e: 0.20563069,
		a: 0.38709893,
		i: 7.00487,
		L: 252.25084,
		r: 0.0000163083872,
		gravParam: 2.2032 * Math.pow(10, 13),
		colour: 0xB0B0B0,
		trackColour: 0x505050,
		center: "sun",
		rotation: 58.6458333,

		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			normalMap: true,
		}
	},
	venus: {
		loPE: 131.53298,
		loAN: 76.68069,
		e: 0.00677323,
		a: 0.72333199,
		i: 3.39471,
		L: 181.97973,
		r: 0.0000404537843,
		gravParam: 3.24859 * Math.pow(10, 14),
		colour: 0xffd9b3,
		trackColour: 0xffbf80,
		center: "sun",
		rotation: 116.75,

		axialTilt: [-0.03633595624, 0.03633595624, -0.99862953475],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			atmo: true,
		}
	},
	earth: {
		loPE: 102.94719,
		loAN: 11.26064,
		e: 0.01671022,
		a: 1,
		i: 0.00005,
		L: 100.46435,
		r: 0.0000426349651,
		gravParam: 3.986004418 * Math.pow(10, 14),
		colour: 0x6666ff,
		trackColour: 0x6666ff,
		center: "sun",
		rotation: 0.997268519,
		rotEpoch: Math.PI * 0.60,

		axialTilt: [0, 0.39777699402, 0.91748213226],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			specularMap: true,
			atmo: true,
		}


	},
	iss: {
		loPE: 320.1639,
		loAN: 263.1947,
		e: 0.0004179,
		a: 4.52947623 * Math.pow(10, -5),
		i: 51.6384,
		L: 180,
		r: 3.3422935611342224e-10,
		gravParam: 0,
		colour: 0x968570,
		trackColour: 0xfff1d5,
		center: "earth",
		satellite: true,
		viewingClass: {
			minorBody: false,
			minorSatellite: true,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			//model: true,
			//glTF: true,
		}

	},
	geo: {
		loPE: 180,
		loAN: 180,
		e: 0,
		a: 0.00028184893,
		i: 0,
		L: 180,
		r: 1e-20,
		gravParam: 0,
		colour: 0x968570,
		trackColour: 0xfff1d5,
		center: "earth",
		satellite: true,
		viewingClass: {
			minorBody: false,
			minorSatellite: true,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}
	},
	"the moon": {
		loPE: 318.15,
		loAN: 125.080,
		e: 0.0549006,
		a: 0.0025695552897999907,
		i: 5.145,
		L: 228.6,
		r: 0.0000116138017,
		gravParam: 4.902801 * Math.pow(10, 12),
		colour: 0x968570,
		trackColour: 0xfff1d5,
		center: "earth",
		rotation: 27.451893572053983,
		rotEpoch: 1,
		tidallyLocked: true,
		axialTilt: [0, -0.01745240643, -0.99984769515],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: true,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			heightMapScale: 30,
		}
	},
	luna: {
		loPE: 318.15,
		loAN: 125.080,
		e: 0.0549006,
		a: 0.0025695552897999907,
		i: 5.145,
		L: 228.6,
		r: 0.0000116138017,
		gravParam: 4.902801 * Math.pow(10, 12),
		colour: 0x968570,
		trackColour: 0xfff1d5,
		center: "earth",
		rotation: 27.451893572053983,
		rotEpoch: 1,
		tidallyLocked: true,
		axialTilt: [0, -0.01745240643, -0.99984769515],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: true,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			heightMapScale: 30,
		}
	},
	"comet 67p": {
		aoPE: 12.694464099565,
		loAN: 50.180045884181,
		e: 0.64058232,
		a: 3.46473750,
		i: 7.04368071,
		M: 92.073462245369,
		r: 2.67383485e-8,
		mass: 9.982 * Math.pow(10, 12),
		//colour: 0xc4cec5,
		colour: 0x171918,
		trackColour: 0x808078,
		center: "sun",
		epoch: new Date(2010, 9, 23, 0, 0, 0, 0),
		rotation: 0.531708333,
		axialTilt: [0.0941841909986137, -0.0785268962411529, 0.99245295341022],
		viewingClass: {
			minorBody: true,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			model: true,
		}


	},
	eros: {
		loPE: 178.79906712581,
		loAN: 304.33018453025,
		e: 0.22260615,
		a: 1.45794695,
		i: 10.82819105,
		L: 207.34212372598,
		r: 5.3476697e-8,
		gravParam: 467541,
		colour: 0xab9e8e,
		trackColour: 0x707070,
		SOI: 0.00000236799,
		center: "sun",

		rotation: 0.2195833,
		axialTilt: [-0.15513883433518022, -0.10594834253099779, 0.9821949352323376],
		viewingClass: {
			minorBody: true,
			minorSatellite: false,
			expanse: true,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			model: true,
		}
	},
	mars: {
		loPE: 336.04084,
		loAN: 49.57854,
		e: 0.09341233,
		a: 1.52366231,
		i: 1.85061,
		L: 355.45332,
		r: 0.0000227075425,
		gravParam: 4.282837 * Math.pow(10, 13),
		colour: 0xff6600,
		trackColour: 0xff944d,
		center: "sun",
		rotation: 1.02749125,
		rotEpoch: 0,

		heightMapScale: 5,
		axialTilt: [0.4460741586578732, -0.053781564787631575, 0.89337863656272],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			normalMap: true,
			atmo: true,
		}

	},
	phobos: {
		aoPE: 150.247,
		loAN: 164.931,
		e: 0.0151,
		a: 0.0000626881,
		i: 1.08,
		M: 92.474,
		r: 0.0000000735305,
		gravParam: 0.0007158 * Math.pow(10, 9),
		colour: 0xa0887e,
		epoch: new Date(1950, 0, 1, 1, 0, 0, 0),
		rotation: 0.31910,
		trackColour: 0xfff1d5,
		center: "mars",
		rotEpoch: Math.PI * (1 / 2),
		axialTilt: [0.4460741586578732, -0.053781564787631575, 0.89337863656272],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			model: true,
			tidallyLocked: true,
		}
	},
	deimos: {
		aoPE: 290.496,
		loAN: 339.600,
		e: 0.0005,
		a: 0.00015681373,
		i: 1.79,
		M: 296.2300,
		r: 0.000000041444,
		gravParam: 0.000098 * Math.pow(10, 9),
		rotation: 1.26244,
		epoch: new Date(1950, 0, 1, 1, 0, 0, 0),
		colour: 0xc1a78c,
		trackColour: 0xfff1d5,
		center: "mars",
		axialTilt: [0.43224207056646835, -0.05273895988650168, 0.9002140826172973],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: true,
			easterEgg: false,
		},
		mapClass: {
			model: true,
			tidallyLocked: true,
		}
	},
	"tesla roadster": {
		loPE: 152.27386035152907,
		loAN: 317.49165095354044,
		e: 0.2654870543099944,
		a: 1.342471905970222,
		i: 1.1167758518947666,
		L: 201.11157387994,
		r: 3.34229356e-11,
		gravParam: Math.pow(10, -10),
		colour: 0xff0000,
		trackColour: 0xff0000,
		center: "sun",
		satellite: true,
		//epoch: new Date(2019, 0, 1, 0, 0, 0, 0),
		viewingClass: {
			minorBody: true,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: true,
		},
		mapClass: {}
	},
	/*vesta: {
		loPE: 151.11163471713,
		loAN: 103.84251223291,
		e: 0.08906674,
		a: 2.36134820,
		i: 7.14040616,
		L: 183.876822474191,
		r: 0.00000350941,
		gravParam: 1.9333925 * Math.pow(10, 15),
		colour: 0x817a6a,
		trackColour: 0x707070,
		SOI: 0.00026239704,
		center: "sun",
		model: true,
		rotation: 0.2226,
	},*/
	"tycho station": {
		loPE: 72.814756865594,
		loAN: 80.314273209733,
		e: 0,
		a: 2.5,
		i: 0,
		L: 180,
		r: 8e-9,
		gravParam: Math.pow(10, -10),
		colour: 0x0066ff,
		trackColour: 0xff3333,
		center: "sun",
		satellite: true,
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: true,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}

	},
	ceres: {
		aoPE: 72.814756865594,
		loAN: 80.314273209733,
		e: 0.07570506,
		a: 2.76813421,
		i: 10.59170527,
		M: 301.6548490,
		r: 0.00000327545,
		gravParam: 6.26325 * Math.pow(10, 10),
		colour: 0xFFFFFF,
		trackColour: 0x909090,
		center: "sun",
		rotation: 0.37808333,
		epoch: new Date(2008, 4, 18, 0, 0, 0, 0),
		viewingClass: {
			minorBody: true,
			minorSatellite: false,
			expanse: true,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			normalMap: true,
		}
	},
	/*pallas: {
		loPE: 309.99921460477,
		loAN: 173.08784381525,
		e: 0.23083485,
		a: 2.77277934,
		i: 34.84116170,
		L: 206.28638093539,
		r: 0.00000327545,
		gravParam: 8.9578031 * Math.pow(10, 14),
		colour: 0x707070,
		trackColour: 0x505050,
		SOI: 0.00022653426,
		center: "sun",
		model: true,
		rotation: 0.325550891667
	},*/
	jupiter: {
		loPE: 100.55615,
		loAN: 14.75385,
		e: 0.04839266,
		a: 5.20336301,
		i: 1.30530,
		L: 34.40438,
		r: 0.000477895,
		gravParam: 1.26686534 * Math.pow(10, 17),
		colour: 0xdd6e00,
		trackColour: 0xc99039,
		center: "sun",
		rotation: 0.41,
		rotationalEpoch: 0,
		axialTilt: [0.03676955262, 0.03676955262, 0.9986],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}

	},
	io: {
		aoPE: 84.129,
		loAN: 43.977,
		e: 0.0041,
		a: 0.00282435945,
		i: 0.036,
		M: 342.021,
		r: 0.000012175975,
		//r: 0.001,
		gravParam: 5959.916 * Math.pow(10, 9),
		colour: 0xffff66,
		trackColour: 0xffff99,
		center: "jupiter",
		rotation: 1.769138,
		epoch: new Date(1997, 0, 1, 16, 0, 0, 0),
		axialTilt: [-0.014599255988641648, -0.03475967618580117, 0.9992890606006035],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			tidallyLocked: true,
		},
	},
	europa: {
		aoPE: 88.970,
		loAN: 219.106,
		e: 0.0094,
		a: 0.004492213,
		i: 0.466,
		M: 171.016,
		r: 0.000010433304,
		gravParam: 3202.739 * Math.pow(10, 9),
		colour: 0xdfbf9f,
		trackColour: 0xecd9c6,
		center: "jupiter",
		rotation: 3.551181,
		epoch: new Date(1997, 0, 1, 16, 0, 0, 0),
		axialTilt: [-0.014390666699817147, -0.03457416102077432, 0.9992985220151406],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			tidallyLocked: true,
		}

	},
	ganymede: {
		aoPE: 192.417,
		loAN: 63.552,
		e: 0.0013,
		a: 0.00715408815,
		i: 0.177,
		M: 317.540,
		r: 0.000017588486,
		gravParam: 9887.834 * Math.pow(10, 9),
		colour: 0xc68c53,
		trackColour: 0xd9b38c,
		center: "jupiter",
		rotation: 7.154553,
		epoch: new Date(1997, 0, 1, 16, 0, 0, 0),
		axialTilt: [-0.013741343223072801, -0.03394797028634311, 0.9993291303668994],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			tidallyLocked: true,
		}
	},
	callisto: {
		aoPE: 52.643,
		loAN: 298.848,
		e: 0.0074,
		a: 0.01258297535,
		i: 0.192,
		M: 181.408,
		r: 0.0000161118603,
		gravParam: 7179.289 * Math.pow(10, 9),
		colour: 0x99ff66,
		trackColour: 0xbbff99,
		center: "jupiter",
		rotation: 16.68902,
		epoch: new Date(1997, 0, 1, 16, 0, 0, 0),
		axialTilt: [-0.010132106535143362, -0.030445595371511813, 0.9994850704936191],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
			tidallyLocked: true,
		}
	},
	saturn: {
		loPE: 113.71504,
		loAN: 92.43194,
		e: 0.05415060,
		a: 9.53707032,
		i: 2.48446,
		L: 49.94432,
		r: 0.00038925688,
		gravParam: 3.7931187 * Math.pow(10, 16),
		colour: 0xffcc66,
		trackColour: 0xffcc99,
		center: "sun",
		rotation: 0.4263888888888889,
		axialTilt: [0.08545121452002576, 0.46335874823661466, 0.8820412464106484],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}
	},
	mimas: {
		aoPE: 185.671,
		loAN: 24.502,
		e: 0.0202,
		a: 0.0012401246,
		i: 1.53,
		M: 15.154,
		r: 0.0000013369,
		gravParam: 8978.19 * Math.pow(10, 9),
		colour: 0x968570,
		trackColour: 0xfff1d5,
		center: "saturn",
		rotation: 0.942422,
		epoch: new Date(2004, 0, 1, 1, 0, 0, 0),
		axialTilt: [0.08545121452002576, 0.46335874823661466, 0.8820412464106484],
		viewingClass: {
			minorBody: false,
			minorSatellite: true,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
		}
	},
	enceladus: {
		aoPE: 93.204,
		loAN: 211.923,
		e: 0.0045,
		a: 0.00159106543,
		i: 0.009,
		M: 197.047,
		r: 0.0000016845159548116483,
		gravParam: 7208006400,
		colour: 0xffffff,
		trackColour: 0xffffff,
		center: "saturn",
		rotation: 1.370218,
		epoch: new Date(2004, 0, 1, 1, 0, 0, 0),
		axialTilt: [0.08545121452002576, 0.46335874823661466, 0.8820412464106484],
		viewingClass: {
			minorBody: false,
			minorSatellite: true,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}
	},
	titan: {
		aoPE: 185.671,
		loAN: 24.502,
		e: 0.0292,
		a: 0.00789140472,
		i: 0.33,
		M: 15.154,
		r: 0.0000172128,
		gravParam: 8978.19 * Math.pow(10, 9),
		colour: 0xffcc00,
		trackColour: 0xffe066,
		center: "saturn",
		rotation: 15.945,
		epoch: new Date(2004, 0, 1, 1, 0, 0, 0),
		axialTilt: [0.08495175712596945, 0.45410208010326153, 0.8868903538809639],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			atmo: true,
		}
	},
	iapetus: {
		aoPE: 271.606,
		loAN: 75.831,
		e: 0.0283,
		a: 0.0238058201,
		i: 14.72,
		M: 356.029,
		r: 4.98670199e-6,
		mass: 18.1 * Math.pow(10, 20),
		colour: 0xffffff,
		trackColour: 0x666666,
		center: "saturn",
		rotation: 79.33,
		rotEpoch: Math.PI,
		axialTilt: [0.0633871624365879, 0.22039452372562077, 0.9733490235008173],
		epoch: new Date(2004, 0, 1, 1, 0, 0, 0),
		viewingClass: {
			minorBody: false,
			minorSatellite: true,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}
	},
	phoebe: {
		aoPE: 345.582,
		loAN: 241.570,
		e: 0.1635,
		a: 0.08655056345,
		i: 175.986,
		M: 287.593,
		r: 0.000000708566,
		gravParam: 553414713.6,
		colour: 0x968570,
		trackColour: 0xfff1d5,
		rotation: 0.3861111111111111,
		epoch: new Date(2004, 0, 1, 1, 0, 0, 0),
		center: "saturn",
		axialTilt: [0.05340976373894771, 0.014271413943494349, 0.9984706925500635],
		viewingClass: {
			minorBody: false,
			minorSatellite: true,
			expanse: true,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			model: true,
		}
	},
	uranus: {
		loPE: 74.22988,
		loAN: 170.96424,
		e: 0.04716771,
		a: 19.19126393,
		i: 0.76986,
		L: 313.23218,
		r: 0.0001695345,
		gravParam: 5.793939 * Math.pow(10, 15),
		colour: 0x99ccff,
		trackColour: 0xb3d9ff,
		center: "sun",
		rotation: 0.7180555555555556,
		axialTilt: [-0.22276184614, -0.96488756229, -0.13917310],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}
	},
	"the ring": {
		loPE: 0.1,
		loAN: 0,
		e: 0,
		a: 22.19126393,
		i: 0,
		L: 313.23218,
		r: 0.000003342293561134223,
		gravParam: 26.3348543,
		colour: 0x0db3bf,
		trackColour: 0x086b72,
		center: "sun",
		rotation: 35606.40728450042,
		rotEpoch: 1.5,
		satellite: true,
		tidallyLocked: true,
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: true,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			surfaceMap: false,
		}
	},
	neptune: {
		loPE: 131.72169,
		loAN: 44.97135,
		e: 0.00858587,
		a: 30.06896348,
		i: 1.76917,
		L: 304.88003,
		r: 0.000164587904,
		gravParam: 6.8365 * Math.pow(10, 15),
		colour: 0x3333cc,
		trackColour: 0xc5cd6,
		center: "sun",
		rotation: 0.67125,
		axialTilt: [0.32353696988, -0.34695092304, 0.88031181186],
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}
	},
	pluto: {
		loPE: (112.59714 + 110.30347),
		loAN: 110.30347,
		e: 0.24880766,
		a: 39.48168677,
		i: 17.14175,
		//i: 0,
		L: (25.2471897 + 112.59714 + 110.30347),
		r: 0.00000793460491460929,
		gravParam: 8.71 * Math.pow(10, 11),
		colour: 0xff9966,
		trackColour: 0xffaa80,
		center: "sun",
		rotation: 6.38725,
		epoch: new Date(2008, 8, 12, 0, 0, 0, 0),
		axialTilt: [0, 0.8688908169, -0.4950037861],
		heightMapScale: 50,
		viewingClass: {
			minorBody: true,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {
			bumpMap: true,
		}
	},
	charon: {
		aoPE: 71.255,
		loAN: 85.187,
		e: 0.0022,
		a: 0.00011722092,
		i: 0.001,
		M: 147.848,
		r: 0.00000404417521,
		gravParam: 108 * Math.pow(10, 9),
		colour: 0xcccccc,
		trackColour: 0xeeeeee,
		center: "pluto",
		rotation: 6.38725,
		epoch: new Date(2004, 0, 1, 1, 0, 0, 0),
		viewingClass: {
			minorBody: false,
			minorSatellite: false,
			expanse: false,
			expanseHide: false,
			easterEgg: false,
		},
		mapClass: {}
	},
};

/* Format of stellar data
	distance: Distance in light years from Sol
	RA: Right ascension in [h,m,s]
	DE: Declination in [degrees, minutes, seconds]
*/

var stars = {
	"Alpha Centauri": {
		distance: 4.367,
		RA: [14, 39, 37], //[12, 39, 37],
		DE: [-60, -50, -2] //[-67, -50, -2],
	},
	"Barnard's Star": {
		distance: 5.96,
		RA: [5, 57, 48.5],
		DE: [-4, -41, -36]
	},
	"Sirius": {
		distance: 8.58,
		RA: [6, 45, 9],
		DE: [-16, -42, 58]
	},
	"Epsilon Eridani": {
		distance: 10.47,
		RA: [03, 32, 55.84496],
		DE: [-9, -27, -29.7312]
	},
	"Tau Ceti": {
		distance: 11.88,
		RA: [01, 44, 5.13],
		DE: [-15, -56, -22.4]
	},
	"Polaris": {
		distance: 433.8,
		RA: [2, 31, 49],
		DE: [89, 15, 50.8]
	},
	"Betelgeuse": {
		distance: 642.5,
		RA: [5, 55, 10],
		DE: [7, 24, 26]
	}
};

