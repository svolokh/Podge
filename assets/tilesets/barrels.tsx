<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="barrels" tilewidth="64" tileheight="64" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <properties>
  <property name="keyframes" type="file" value="barrels/barrel.json"/>
 </properties>
 <tile id="0" type="barrel">
  <properties>
   <property name="keyframes" type="file" value="barrels/barrel.json"/>
  </properties>
  <image width="64" height="64" source="barrels/barrel.png"/>
  <objectgroup draworder="index">
   <object id="9" x="0" y="16">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 19,-6 21,39 0,32"/>
   </object>
   <object id="10" x="44" y="10">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 20,6 20,38 0,44"/>
   </object>
   <object id="12" x="19" y="10">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 11,-2 26,0 25,45 12,47 2,45"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
