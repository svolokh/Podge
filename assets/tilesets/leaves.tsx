<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="leaves" tilewidth="320" tileheight="128" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="leaf">
  <properties>
   <property name="ccw" type="bool" value="true"/>
   <property name="hits" type="int" value="3"/>
   <property name="keyframes" type="file" value="leaves/leaf.json"/>
  </properties>
  <image width="320" height="128" source="leaves/leaf.png"/>
  <objectgroup draworder="index">
   <object id="11" x="0" y="58">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="1,0 43,-23 81,-31 80,43 54,38 18,22 0,9"/>
   </object>
   <object id="16" x="81" y="32">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 70,-12 72,77 0,65"/>
   </object>
   <object id="17" x="151" y="20">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 14,0 16,6 9,13 1,14"/>
   </object>
   <object id="18" x="160" y="95">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 6,9 2,14 -7,14 -7,-1"/>
   </object>
   <object id="19" x="162" y="33">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 37,-7 40,70 -9,61 -10,2"/>
   </object>
   <object id="20" x="199" y="26">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 35,0 38,74 17,77 3,77"/>
   </object>
   <object id="21" x="233" y="26">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 12,4 13,11 8,15 2,15"/>
   </object>
   <object id="22" x="241" y="88">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 5,5 4,9 -5,13 -4,-2"/>
   </object>
   <object id="24" x="243" y="40">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 23,2 29,46 -5,47 -8,0"/>
   </object>
   <object id="25" x="267" y="41">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 31,8 39,34 6,45"/>
   </object>
   <object id="26" x="299" y="49">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 18,11 20,15 7,26"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
