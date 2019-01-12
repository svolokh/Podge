<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="leaf" tilewidth="320" tileheight="128" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="leaf">
  <properties>
   <property name="ccw" type="bool" value="true"/>
   <property name="hits" type="int" value="3"/>
   <property name="keyframes" type="file" value="leaf.json"/>
  </properties>
  <image width="320" height="128" source="leaf.png"/>
  <objectgroup draworder="index">
   <object id="11" x="0" y="58">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="1,0 43,-23 81,-31 80,43 54,38 18,22 0,9"/>
   </object>
   <object id="12" x="82" y="33">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 54,-13 83,-13 79,-2 79,63 84,73 63,75 -1,64"/>
   </object>
   <object id="13" x="163" y="33">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,-2 43,-8 45,69 -2,64"/>
   </object>
   <object id="14" x="208" y="25">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 29,1 38,7 33,14 33,63 38,70 26,76 0,77"/>
   </object>
   <object id="15" x="243" y="39">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 24,2 55,9 76,24 58,39 26,48 -3,48"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
