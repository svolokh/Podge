<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="windmills" tilewidth="320" tileheight="320" tilecount="2" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="obstacle">
  <properties>
   <property name="keyframes" type="file" value="windmill1.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="true"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="192" height="192" source="windmill1.png"/>
  <objectgroup draworder="index">
   <object id="3" x="64" y="0" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
   <object id="6" x="128" y="64" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
   <object id="11" x="64" y="128" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
   <object id="14" x="0" y="64" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="1" type="obstacle">
  <properties>
   <property name="keyframes" type="file" value="windmill2.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="true"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="320" height="320" source="windmill2.png"/>
  <objectgroup draworder="index">
   <object id="3" x="0" y="128" width="129" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
   <object id="4" x="128" y="0" width="64" height="128">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
   <object id="5" x="192" y="128" width="128" height="63">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
   <object id="6" x="128" y="193" width="64" height="127">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
</tileset>
