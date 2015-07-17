	void aPID(float encPos, CANTalon *talon, float encTar, int vP, float &i)
	{
		bool rev;
  //Which reverse states for which drive module
  //front left=1, front right=2, back left=3, back right=4
		switch (vP)
		{
			case 1:
				rev = flRev;
				break;
			case 2:
				rev = frRev;
				break;
			case 3:
				rev = blRev;
				break;
			case 4:
				rev = brRev;
				break;
		}
		//error for PID (proportional integral derivative)-feedback loop
		aError = encTar - encPos;
		//changes encoder values based on if one is rotating,translating and takes into account rev
		
		//fixes sigularity issues
		//if wheel closer to certain degree in reverse, changes state (rev)
		if (!mix)
		{
			if (encTar > 180)
			{
				encTar -= 360;
			}
			if (encPos > 180)
			{
				encPos -= 360;
			}
			if (rev)
			{
				if (encTar > 0)
				{
					encTar += 180;
				}
				else
				{
					encTar -= 180;
				}
			}
			if (encTar - encPos > 90 && encTar - encPos < 270)
			{
				rev = !rev;
				encTar = encTar - 180;
			}
			else if (encTar - encPos < -90 && encTar - encPos > -270)
			{
				rev = !rev;
				encTar = encTar + 180;
			}
			if (encTar < 0)
			{
				encTar += 360;
			}
			if (encPos < 0)
			{
				encPos += 360;
			}
		}
		//recalculate error
		//if position is left of target, fix singularity over 0'
		aError = encTar - encPos;
		if (encPos > 270 && encPos < 360 && encTar > 0 && encTar < 90)
		{
			aError = encTar + 360 - encPos;
		}
		//if target is left of position, fix singularity over 0'
		else if (encTar > 270 && encTar < 360 && encPos > 0 && encPos < 90)
		{
			aError = encTar - 360 - encPos;
		}
		//makes more accurate positioning of wheels
		//I for PID stuff
		i += PID_I * aError;
		if (aError < 2) i = 0;
		iPID(aError, talon, i);
		if (aError < 10) mag = 0;

		//again sets states
		switch (vP)
		{
			case 1:
				flRev = rev;
				break;
			case 2:
				frRev = rev;
				break;
			case 3:
				blRev = rev;
				break;
			case 4:
				brRev = rev;
				break;
		}

	}
