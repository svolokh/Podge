<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="walls" tilewidth="64" tileheight="64" tilecount="7" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="wall">
  <properties>
   <property name="keyframes" type="file" value="wall1.json"/>
  </properties>
  <image width="64" height="64" source="wall1.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="1" type="wall">
  <properties>
   <property name="keyframes" type="file" value="wall2.json"/>
  </properties>
  <image width="64" height="64" source="wall2.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 32,32 64,0"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="2" type="wall">
  <properties>
   <property name="keyframes" type="file" value="wall3.json"/>
  </properties>
  <image width="64" height="64" source="wall3.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 32,-32 64,0"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="3" type="wall">
  <properties>
   <property name="keyframes" type="file" value="wall4.json"/>
  </properties>
  <image width="64" height="64" source="wall4.png"/>
  <objectgroup draworder="index">
   <object id="2" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,64 0,64"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="4" type="wall">
  <properties>
   <property name="keyframes" type="file" value="wall5.json"/>
  </properties>
  <image width="64" height="64" source="wall5.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,-64 64,0"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="5" type="wall">
  <properties>
   <property name="keyframes" type="file" value="wall6.json"/>
  </properties>
  <image width="64" height="64" source="wall6.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,-64 0,-64"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="6" type="wall">
  <properties>
   <property name="keyframes" type="file" value="wall7.json"/>
  </properties>
  <image width="64" height="64" source="wall7.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,64 64,0"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
